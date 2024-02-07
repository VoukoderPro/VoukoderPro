#include "voukoder_container.h"
#include "json.hpp"
#include "utils.h"

#include <boost/filesystem/path.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/algorithm/string/join.hpp>

using namespace IOPlugin;
using json = nlohmann::json;
namespace fs = boost::filesystem;

const uint8_t VoukoderFormat::s_UUID[] = { 0x69, 0xfd, 0xa3, 0x10, 0x8d, 0x8e, 0x42, 0x7f, 0xb0, 0x32, 0x87, 0xb6, 0x3d, 0x10, 0xe8, 0x78 };

void rat_approx(double f, int64_t md, int64_t* num, int64_t* denom)
{
    int64_t a, h[3] = { 0, 1, 0 }, k[3] = { 1, 0, 0 };
    int64_t x, d, n = 1;
    int i, neg = 0;

    if (md <= 1) { *denom = 1; *num = (int64_t)f; return; }

    if (f < 0) { neg = 1; f = -f; }

    while (f != floor(f)) { n <<= 1; f *= 2; }
    d = (int64_t)f;

    for (i = 0; i < 64; i++) {
        a = n ? d / n : 0;
        if (i && !a) break;

        x = d; d = n; n = x % n;

        x = a;
        if (k[1] * a + k[0] >= md) {
            x = (md - k[0]) / k[1];
            if (x * 2 >= a || k[1] >= md)
                i = 65;
            else
                break;
        }

        h[2] = x * h[1] + h[0]; h[0] = h[1]; h[1] = h[2];
        k[2] = x * k[1] + k[0]; k[0] = k[1]; k[1] = k[2];
    }
    *denom = k[1];
    *num = neg ? -h[1] : h[1];
}

class DummyTrackWriter : public IPluginTrackBase, public IPluginTrackWriter
{
public:
    DummyTrackWriter(VoukoderFormat* p_pContainer, uint32_t p_TrackIdx, bool p_IsVideo): IPluginTrackBase(p_pContainer),
        m_TrackIdx(p_TrackIdx), m_IsVideo(p_IsVideo)
    {}
    virtual ~DummyTrackWriter() = default;

    VoukoderPro::config props;
    
public:
    virtual StatusCode DoWrite(HostBufferRef* p_pBuf)
    {
        VoukoderFormat* pContainer = dynamic_cast<VoukoderFormat*>(m_pContainer);
        assert(pContainer != NULL);

        return m_IsVideo ? pContainer->WriteVideo(m_TrackIdx, p_pBuf): pContainer->WriteAudio(m_TrackIdx, p_pBuf);
    }

private:
    uint32_t m_TrackIdx;
    bool m_IsVideo;
};

StatusCode VoukoderFormat::s_Register(HostListRef* p_pList)
{
    HostPropertyCollectionRef containerInfo;
    if (!containerInfo.IsValid())
        return errAlloc;

    containerInfo.SetProperty(pIOPropUUID, propTypeUInt8, VoukoderFormat::s_UUID, 16);

    const char* pContainerName = "VoukoderPro";
    containerInfo.SetProperty(pIOPropName, propTypeString, pContainerName, static_cast<int>(strlen(pContainerName)));

    const uint32_t mediaType = (mediaAudio | mediaVideo);
    containerInfo.SetProperty(pIOPropMediaType, propTypeUInt32, &mediaType, 1);
    containerInfo.SetProperty(pIOPropContainerExt, propTypeString, "voukoderpro", 11);

    if (!p_pList->Append(&containerInfo))
        return errFail;

    return errNone;
}

VoukoderFormat::VoukoderFormat()
{}

VoukoderFormat::~VoukoderFormat()
{}

StatusCode VoukoderFormat::DoInit(HostPropertyCollectionRef* p_pProps)
{
    if (vkdrpro)
        return errNone;

    // Create VoukoderPro instance
    int ret = 0;
    if ((ret = voukoderpro_create()) < 0)
    {
        switch (ret)
        {
        case -1:
            g_Log(logLevelError, "VoukoderPro Plugin :: VOUKODERPRO_HOME environment variable is not set.");
            break;

        case -2:
            g_Log(logLevelError, "VoukoderPro Plugin :: Unable to set DLL search path.");
            break;

        case -3:
            g_Log(logLevelError, "VoukoderPro Plugin :: Unable to load voukoderpro library.");
            break;

        default:
            g_Log(logLevelError, "VoukoderPro Plugin :: Unknown error #%d", ret);
        }
        return errNoCodec;
    }

    if (vkdrpro->init() < 0)
    {
        g_Log(logLevelError, "VoukoderPro Plugin :: Initialization failed!");

        return errFail;
    }

    return errNone;
}

StatusCode VoukoderFormat::DoOpen(HostPropertyCollectionRef* p_pProps)
{
    // Project structure
    project[VoukoderPro::pPropApplication] = "DaVinci Resolve Studio";
    project[VoukoderPro::pPropApplicationVersion] = "0.0.0.0";

    // File name
    std::string path;
    p_pProps->GetString(pIOPropPath, path);
    project[VoukoderPro::pPropFilename] = path;

    json tracks = json::array();
    for (auto track : m_TrackVec)
        project.tracks.push_back(track->props);

    // Start the VoukoderPro session
    int ret = 0;
    if ((ret = vkdrpro->open(project)) < 0)
    {
        g_Log(logLevelInfo, "VoukoderPro Plugin :: Unable to open VoukoderPro (Error code #%d)", ret);
        return errNoCodec;
    }

    vkdrpro->event("export", {
        { "dimension1", "DaVinci Resolve Studio" },
        { "dimension2", "DaVinci Resolve Studio " + GetAppVersionFromFile() }
    });

    return errNone;
}

StatusCode VoukoderFormat::DoAddTrack(HostPropertyCollectionRef* p_pProps, HostPropertyCollectionRef* p_pCodecProps, IPluginTrackBase** p_pTrack)
{
    uint32_t mediaType = 0;
    if (!p_pProps->GetUINT32(pIOPropMediaType, mediaType) || ((mediaType != mediaVideo) && (mediaType != mediaAudio)))
        return errInvalidParam;

    DummyTrackWriter* pTrack = new DummyTrackWriter(this, static_cast<uint32_t>(m_TrackVec.size()), mediaType == mediaVideo);
    pTrack->Retain();

    *p_pTrack = pTrack;

    if (mediaType == mediaVideo)
    {
        // Fetch all available scenes
        std::vector<std::shared_ptr<VoukoderPro::SceneInfo>> scenes;
        vkdrpro->sceneManager()->load(scenes);

        int32_t sceneHash;
        p_pCodecProps->GetINT32("vkdrpro_scene", sceneHash);

        for (const auto& sceneInfo : scenes)
        {
            if (sceneHash == hash(sceneInfo->name.c_str()))
            {
                vkdrpro->setScene(sceneInfo);
                break;
            }
        }

        pTrack->props[VoukoderPro::pPropType] = "video";

        // Frame size
        uint32_t width, height;
        if (!p_pProps->GetUINT32(pIOPropWidth, width) || !p_pProps->GetUINT32(pIOPropHeight, height))
        {
            g_Log(logLevelError, "VoukoderPro Plugin :: Width/Height not set when encoding the frame");
            return errNoParam;
        }
        
        pTrack->props[VoukoderPro::pPropWidth] = static_cast<int>(width);
        pTrack->props[VoukoderPro::pPropHeight] = static_cast<int>(height);

        // Video time base
        PropertyType propType;
        const void* pVal = NULL;
        int numVals = 0;
        if ((errNone == p_pProps->GetProperty(pIOPropFrameRate, &propType, &pVal, &numVals)) &&
            (propType == propTypeUInt32) && (pVal != NULL) && (numVals == 2))
        {
            const uint32_t* pFrameRate = static_cast<const uint32_t*>(pVal);
            pTrack->props[VoukoderPro::pPropTimebaseNum] = static_cast<int>(pFrameRate[1]);
            pTrack->props[VoukoderPro::pPropTimebaseDen] = static_cast<int>(pFrameRate[0]);
        }

        // Aspect ratio
        double par = 1.0;
        p_pProps->GetDouble(pIOPropPAR, par);
        int64_t num = 0, den = 0;
        rat_approx(par, 65535, &num, &den);
        pTrack->props[VoukoderPro::pPropAspectNum] = static_cast<int>(num);
        pTrack->props[VoukoderPro::pPropAspectDen] = static_cast<int>(den);

        // Field order
        uint8_t fieldOrder;
        p_pProps->GetUINT8(pIOPropFieldOrder, fieldOrder);
        if (fieldOrder == FieldOrder::fieldTop)
            pTrack->props[VoukoderPro::pPropFieldOrder] = "tff";
        else if (fieldOrder == FieldOrder::fieldBottom)
            pTrack->props[VoukoderPro::pPropFieldOrder] = "bff";
        else if (fieldOrder == FieldOrder::fieldNone || fieldOrder == FieldOrder::fieldProgressive)
            pTrack->props[VoukoderPro::pPropFieldOrder] = "progressive";
        else
            return errInvalidParam;

        // Pixel format
        uint32_t bitDepth;
        p_pProps->GetUINT32(pIOPropBitsPerSample, bitDepth);

        uint32_t colorModel;
        p_pProps->GetUINT32(pIOPropColorModel, colorModel);
        if (colorModel == clrRGBA)
        {
            pTrack->props[VoukoderPro::pPropFormat] = "rgba";
        }
        else if (colorModel == clrYUVp || colorModel == clrRGBAp)
        {
            uint8_t hSubSampling, vSubSampling;
            p_pProps->GetUINT8(pIOPropHSubsampling, hSubSampling);
            p_pProps->GetUINT8(pIOPropVSubsampling, vSubSampling);

            uint32_t strides;
            p_pProps->GetUINT32(pIOPropStride, strides);

            if (hSubSampling == 2 && vSubSampling == 2 && bitDepth == 8)
            {
                pTrack->props[VoukoderPro::pPropFormat] = "yuv420p";
            }
            else if (hSubSampling == 2 && vSubSampling == 1 && bitDepth == 8)
            {
                pTrack->props[VoukoderPro::pPropFormat] = "yuv422p";
            }
            else if (hSubSampling == 1 && vSubSampling == 1 && bitDepth == 8)
            {
                pTrack->props[VoukoderPro::pPropFormat] = "yuv444p";
            }
            else if (hSubSampling == 1 && vSubSampling == 1 && bitDepth == 10)
            {
                pTrack->props[VoukoderPro::pPropFormat] = "yuv444p10le";
            }
            else if (hSubSampling == 1 && vSubSampling == 1 && bitDepth == 16)
            {
                pTrack->props[VoukoderPro::pPropFormat] = "yuv444p16le";
            }
        }
        else if (colorModel == clrV210)
        {
            pTrack->props[VoukoderPro::pPropFormat] = "v210";
        }
        else if (colorModel == clrUYVY)
        {
            pTrack->props[VoukoderPro::pPropFormat] = "uyvy422";
        }
        else if (colorModel == clrAYUV)
        {
            pTrack->props[VoukoderPro::pPropFormat] = "ayuv64le";
        }

        // Color range
        uint8_t colorRange;
        if (p_pProps->GetUINT8(pIOPropDataRange, colorRange))
            pTrack->props[VoukoderPro::pPropColorRange] = colorRange ? "pc" : "tv";

        // Color space
        if ((errNone == p_pProps->GetProperty(pIOColorMatrix, &propType, &pVal, &numVals)) &&
            (propType == propTypeInt16) && (pVal != NULL) && (numVals == 1))
        {
            const int16_t* colorSpace = static_cast<const int16_t*>(pVal);
            switch (*colorSpace)
            {
            case 0:
                pTrack->props[VoukoderPro::pPropColorSpace] = "rgb";
                break;
            case 1:
                pTrack->props[VoukoderPro::pPropColorSpace] = "bt709";
                break;
            case 4:
                pTrack->props[VoukoderPro::pPropColorSpace] = "fcc";
                break;
            case 5:
                pTrack->props[VoukoderPro::pPropColorSpace] = "bt470bg";
                break;
            case 6:
                pTrack->props[VoukoderPro::pPropColorSpace] = "smpte170m";
                break;
            case 7:
                pTrack->props[VoukoderPro::pPropColorSpace] = "smpte240m";
                break;
            case 8:
                pTrack->props[VoukoderPro::pPropColorSpace] = "ycocg";
                break;
            case 9:
                pTrack->props[VoukoderPro::pPropColorSpace] = "bt2020nc";
                break;
            case 10:
                pTrack->props[VoukoderPro::pPropColorSpace] = "bt2020c";
                break;
            default:
                pTrack->props[VoukoderPro::pPropColorSpace] = "unknown";
                break;
            }
        }

        // Color primaries
        if ((errNone == p_pProps->GetProperty(pIOPropColorPrimaries, &propType, &pVal, &numVals)) &&
            (propType == propTypeInt16) && (pVal != NULL) && (numVals == 1))
        {
            auto colorPrim = static_cast<const int16_t*>(pVal);
            switch (*colorPrim)
            {
            case 1:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "bt709";
                break;
            case 4:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "bt470m";
                break;
            case 5:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "bt470bg";
                break;
            case 6:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "smpte170m";
                break;
            case 7:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "smpte240m";
                break;
            case 8:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "film";
                break;
            case 9:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "bt2020";
                break;
            case 10:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "smpte428";
                break;
            case 11:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "smpte431";
                break;
            case 12:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "smpte432";
                break;
            default:
                pTrack->props[VoukoderPro::pPropColorPrimaries] = "unknown";
                break;
            }
        }

        // Color TRC
        if ((errNone == p_pProps->GetProperty(pIOTransferCharacteristics, &propType, &pVal, &numVals)) &&
            (propType == propTypeInt16) && (pVal != NULL) && (numVals == 1))
        {
            auto colorTrc = static_cast<const int16_t*>(pVal);
            switch (*colorTrc)
            {
            case 1:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "bt709";
                break;
            case 4:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "gamma22";
                break;
            case 5:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "gamma28";
                break;
            case 6:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "smpte170m";
                break;
            case 7:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "smpte240m";
                break;
            case 8:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "linear";
                break;
            case 9:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "log100";
                break;
            case 10:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "log316";
                break;
            case 11:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "iec61966-2-4";
                break;
            case 12:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "bt1361e";
                break;
            case 13:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "iec61966-2-1";
                break;
            case 14:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "bt2020-10";
                break;
            case 15:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "bt2020-12";
                break;
            case 16:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "smpte2084";
                break;
            case 17:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "smpte428";
                break;
            case 18:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "arib-std-b67";
                break;
            default:
                pTrack->props[VoukoderPro::pPropColorTransfer] = "unknown";
                break;
            }
        }

        // Timecode
        double timecode = 0.0;
        p_pProps->GetDouble(pIOPropStartTime, timecode);
        const int numerator = std::get<int>(pTrack->props[VoukoderPro::pPropTimebaseNum]);
        const int denominator = std::get<int>(pTrack->props[VoukoderPro::pPropTimebaseDen]);
        pTrack->props[VoukoderPro::pPropTimecode] = GetTimecode(timecode, numerator, denominator);

        // Chapters
        std::string markerColor;
        p_pCodecProps->GetString("vkdrpro_markers", markerColor);
        if (!markerColor.empty())
        {
            g_Log(logLevelWarn, "VoukoderPro Plugin :: Using marker color: %s", markerColor.c_str());

            // get all markers data from track props and find those matching codec settings color
            PropertyType type = propTypeNull;
            const void* pVal = NULL;
            int numVals = 0;
            const StatusCode err = p_pProps->GetProperty(pIOPropMarkersBlob, &type, &pVal, &numVals);
            if ((err == errNone) && (type == propTypeUInt8) && (numVals > 0))
            {
                HostMarkersMap markers;
                if (markers.FromBuffer(static_cast<const uint8_t*>(pVal), numVals))
                {
                    std::vector<std::string> chapters;

                    const std::map<double, HostMarkerInfo>& markersMap = markers.GetMarkersMap();
                    for (auto it = markersMap.begin(); it != markersMap.end(); ++it)
                    {
                        const HostMarkerInfo& marker = it->second;
                        if (marker.GetColor() == markerColor && marker.IsValid())
                        {
                            const std::string chapter = marker.GetName() + ";" + std::to_string(GetFrames(marker.GetPositionSeconds(), numerator, denominator));
                            chapters.push_back(chapter);
                        }
                    }

                    pTrack->props[VoukoderPro::pPropChapters] = boost::algorithm::join(chapters, ",");
                }
            }
        }
    }
    else if (mediaType == mediaAudio)
    {
        pTrack->props[VoukoderPro::pPropType] = "audio";

        // Number of channels
        uint32_t channels;
        p_pProps->GetUINT32(pIOPropNumChannels, channels);
        pTrack->props[VoukoderPro::pPropChannelCount] = static_cast<int>(channels);

        // Sampling rate
        uint32_t samplingRate = 0;
        p_pProps->GetUINT32(pIOPropSamplingRate, samplingRate);
        pTrack->props[VoukoderPro::pPropSamplingRate] = static_cast<int>(samplingRate);

        // Channel layout
        uint32_t ch_layout;
        p_pProps->GetUINT32(pIOPropAudioChannelLayout, ch_layout);
        switch (ch_layout)
        {
        case AudioChannelLayout::audLayoutNone:
        {
            std::vector<std::string> layout;
            for (int i = 0; i < channels; i++)
                layout.push_back("mono");

            pTrack->props[VoukoderPro::pPropChannelLayout] = boost::algorithm::join(layout, "+");
            break;
        }

        case AudioChannelLayout::audLayoutMono:
            pTrack->props[VoukoderPro::pPropChannelLayout] = "mono";
            break;

        case AudioChannelLayout::audLayoutStereo:
            pTrack->props[VoukoderPro::pPropChannelLayout] = "stereo";
            break;

        case AudioChannelLayout::audLayoutGeneric5_1:
            pTrack->props[VoukoderPro::pPropChannelLayout] = "5.1";
            break;

        case AudioChannelLayout::audLayoutGeneric7_1:
            pTrack->props[VoukoderPro::pPropChannelLayout] = "7.1";
            break;

        default:
            return errInvalidParam;
        };

        // Sample bit depth
        uint32_t bitDepth = 0;
        p_pProps->GetUINT32(pIOPropBitDepth, bitDepth);

        uint8_t isFloat = 0; // 0 - integer data, 1 - floating point data
        p_pProps->GetUINT8(pIOPropIsFloat, isFloat);

        // Sample format
        if (isFloat)
            pTrack->props[VoukoderPro::pPropFormat] = "flt";
        else if (bitDepth == 16)
            pTrack->props[VoukoderPro::pPropFormat] = "s16";
        else if (bitDepth == 24) {
            pTrack->props[VoukoderPro::pPropFormat] = "s32";
            pTrack->props[VoukoderPro::pPropOriginalFormat] = "s24";
        }
        else if (bitDepth == 32)
            pTrack->props[VoukoderPro::pPropFormat] = "s32";
    }

    m_TrackVec.push_back(pTrack);

    return errNone;
}

StatusCode VoukoderFormat::DoClose()
{
    // Free tracks
    for (size_t i = 0; i < m_TrackVec.size(); ++i)
        m_TrackVec[i]->Release();

    m_TrackVec.clear();

    // Close VoukoderPro
    if (vkdrpro->close() < 0)
        return errFail;

    return errNone;
}

StatusCode VoukoderFormat::WriteVideo(uint32_t p_TrackIdx, HostBufferRef* p_pBuf)
{
    if (p_pBuf == NULL)
        return errNone;

    DummyTrackWriter* track = m_TrackVec.at(p_TrackIdx);

    char* pBuf = NULL;
    size_t bufSize = 0;
    if (p_pBuf->LockBuffer(&pBuf, &bufSize))
    {
        int64_t frame;
        p_pBuf->GetINT64(pIOPropDTS, frame);

        uint8_t* buf = reinterpret_cast<uint8_t*>(const_cast<char*>(pBuf));

        uint8_t* buffer[3];
        int linesize[3];

        const int width = std::get<int>(track->props[VoukoderPro::pPropWidth]);
        const int height = std::get<int>(track->props[VoukoderPro::pPropHeight]);

        int offset = width * height;
        
        const std::string format = std::get<std::string>(track->props[VoukoderPro::pPropFormat]);
        if (format == "yuv420p")
        {
            buffer[0] = buf;
            buffer[1] = buf + offset;
            buffer[2] = buf + offset + offset / 4;
            linesize[0] = width;
            linesize[1] = linesize[2] = width / 2;
        }
        else if (format == "yuv422p")
        {
            buffer[0] = buf;
            buffer[1] = buf + offset;
            buffer[2] = buf + offset + offset / 2;
            linesize[0] = width;
            linesize[1] = linesize[2] = width / 2;
        }
        else if (format == "yuv444p")
        {
            buffer[0] = buf;
            buffer[1] = buf + offset;
            buffer[2] = buf + offset + offset;
            linesize[0] = linesize[1] = linesize[2] = width;
        }
        else if (format == "yuv444p10le")
        {
            offset *= 2;
            buffer[0] = buf;
            buffer[1] = buf + offset;
            buffer[2] = buf + offset + offset;
            linesize[0] = linesize[1] = linesize[2] = width * 2;
        }
        else if (format == "yuv444p16le")
        {
            offset *= 2;
            buffer[0] = buf;
            buffer[1] = buf + offset;
            buffer[2] = buf + offset + offset;
            linesize[0] = linesize[1] = linesize[2] = width * 2;
        }
        else if (format == "uyvy422" || format == "rgba" || format == "ayuv64le")
        {
            buffer[0] = buf;
            linesize[0] = bufSize / height;
            linesize[1] = linesize[2] = 0;
        }

        // Send one single frame to the server
        int ret = 0;
        if (format == "v210")
            ret = vkdrpro->writeCompressedVideoFrame(p_TrackIdx, frame, buf, bufSize / height);
        else
            ret = vkdrpro->writeVideoFrame(p_TrackIdx, frame, buffer, linesize);

        p_pBuf->UnlockBuffer();

        if (ret < 0)
        {
            g_Log(logLevelWarn, "VoukoderPro Plugin :: Failed writing video frame.");
            return errFail;
        }
    }

    return errNone;
}

StatusCode VoukoderFormat::WriteAudio(uint32_t p_TrackIdx, HostBufferRef* p_pBuf)
{
    if (p_pBuf == NULL)
        return errNone;

    DummyTrackWriter* track = m_TrackVec.at(p_TrackIdx);

    char* pBuf = NULL;
    size_t bufSize = 0;
    if (p_pBuf->LockBuffer(&pBuf, &bufSize))
    {
        // We always get packed buffers
        uint8_t* buffer[1];
        buffer[0] = reinterpret_cast<uint8_t*>(const_cast<char*>(pBuf));

        // Send audio samples to the server
        int ret = vkdrpro->writeAudioSamples(p_TrackIdx, buffer, static_cast<int>(bufSize));

        p_pBuf->UnlockBuffer();

        if (ret < 0)
        {
            g_Log(logLevelWarn, "VoukoderPro Plugin :: Failed writing audio samples.");
            return errFail;
        }
    }

    return errNone;
}

long long VoukoderFormat::GetFrames(double seconds, double n, double d)
{
    return std::round(seconds * d / n);
}

std::string VoukoderFormat::GetTimecode(double seconds, double n, double d)
{
    const double frames = std::round(seconds * d / n);

    if (n == 1001)
    {
        n = 1;
        d /= 1000;

        seconds = frames / d;
    }
    
    std::stringstream timecode;

    // HRS
    const int hrs = std::floor(seconds / 3600.0);
    seconds -= hrs * 3600;
    timecode << std::setw(2) << std::setfill('0') << hrs << ':';

    // MIN
    const int mins = std::floor(seconds / 60.0);
    seconds -= mins * 60;
    timecode << std::setw(2) << std::setfill('0') << mins << ':';

    // SEC
    const int secs = std::floor(seconds);
    seconds -= secs;
    timecode << std::setw(2) << std::setfill('0') << secs << ':';

    // FRM
    timecode << std::setw(2) << std::setfill('0') << std::round((seconds * d) / n);

    return timecode.str();
}
