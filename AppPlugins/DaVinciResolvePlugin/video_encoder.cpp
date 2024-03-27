#include "video_encoder.h"

#include <assert.h>
#include <cstring>
#include <vector>
#include <stdint.h>
#include <boost/process.hpp>
#include <algorithm>
#include <iostream>

#include "voukoderpro.hpp"

namespace bp = boost::process;

using json = nlohmann::json;

const uint8_t VideoCodec::s_UUID_420_8[] = { 0x94, 0xa7, 0x85, 0xc2, 0x3d, 0x20, 0x47, 0x2e, 0xab, 0x23, 0x5a, 0xf6, 0x63, 0x86, 0xe6, 0x2b };
const uint8_t VideoCodec::s_UUID_422_8[] = { 0xe7, 0xcf, 0x16, 0x5d, 0xd8, 0x2b, 0x4e, 0x47, 0x9d, 0x48, 0x5d, 0x15, 0x35, 0x5d, 0x3f, 0x66 };
const uint8_t VideoCodec::s_UUID_444_8[] = { 0x84, 0x11, 0x3e, 0x24, 0xaa, 0xc4, 0x4a, 0xe6, 0xb8, 0x2a, 0x0e, 0xe8, 0xd1, 0x4b, 0x48, 0x6d };
const uint8_t VideoCodec::s_UUID_422_10[] = { 0x64, 0xe9, 0x54, 0xaa, 0x31, 0x06, 0x44, 0xea, 0xba, 0xb2, 0x90, 0x00, 0x5c, 0x24, 0xb3, 0x01 };
const uint8_t VideoCodec::s_UUID_444_10[] = { 0x18, 0x5a, 0x79, 0x6e, 0xf7, 0x75, 0x4e, 0xc3, 0x8d, 0x16, 0x11, 0x17, 0x49, 0x89, 0xfa, 0x0f };
const uint8_t VideoCodec::s_UUID_444_16[] = { 0xe7, 0xe4, 0xda, 0x2e, 0xec, 0xdf, 0x43, 0x3e, 0xb4, 0x85, 0xc3, 0xe6, 0xce, 0x16, 0x4b, 0xd4 };
const uint8_t VideoCodec::s_UUID_RGBA_8[] = { 0x88, 0x94, 0x36, 0xe8, 0xb3, 0xa7, 0x47, 0xcf, 0x89, 0x76, 0xdf, 0x4b, 0xa4, 0x2d, 0x90, 0x13 };
const uint8_t VideoCodec::s_UUID_RGBA_16[] = { 0x0f, 0xaf, 0x7c, 0x8d, 0x11, 0xb8, 0x4d, 0x1f, 0x9e, 0x8b, 0x55, 0xe3, 0xdb, 0x6a, 0x0a, 0x3f };
const uint8_t VideoCodec::s_UUID_AYUV_16[] = { 0xfb, 0x35, 0x8c, 0xe2, 0x4d, 0xbd, 0x4b, 0x97, 0x98, 0x33, 0x08, 0x2d, 0xc7, 0x9f, 0x36, 0xe0 };

class UISettingsController
{
public:
    UISettingsController()
    {
        InitDefaults();
    }

    explicit UISettingsController(const HostCodecConfigCommon& p_CommonProps)
        : m_CommonProps(p_CommonProps)
    {
        InitDefaults();
    }

    ~UISettingsController()
    {}

    void Load(IPropertyProvider* p_pValues)
    {
        p_pValues->GetINT32("vkdrpro_scene", m_scene);
        p_pValues->GetString("vkdrpro_markers", m_MarkerColor);

        uint8_t val8 = 0;
        p_pValues->GetUINT8("vkdrpro_configure", val8);
        if (val8 != 0)
        {
            boost::function<pluginapi_create_t> factory;
            std::shared_ptr<VoukoderPro::IClient> vkdrpro;

            try
            {
                factory = VOUKODERPRO_CREATE_INSTANCE;
                vkdrpro = factory();

                if (!vkdrpro)
                {
                    g_Log(logLevelError, "VoukoderPro Plugin :: Failed to create VoukoderPro instance. (1)");
                    return;
                }
            }
            catch (boost::system::system_error e)
            {
                g_Log(logLevelError, "VoukoderPro Plugin :: Failed to create VoukoderPro instance. (2)");
                return;
            }

            if (vkdrpro->init() < 0)
            {
                g_Log(logLevelError, "VoukoderPro Plugin :: Failed to initialize VoukoderPro.");
                return;
            }

            // Fetch all available scenes
            std::vector<std::shared_ptr<VoukoderPro::SceneInfo>> scenes;
            vkdrpro->sceneManager()->load(scenes);

            std::string name = "";

            for (const auto& sceneInfo : scenes)
            {
                if (m_scene == hash(sceneInfo->name.c_str()))
                {
                    name = sceneInfo->name;
                    break;
                }
            }

            vkdrpro->configure(name);
        }
    }

    StatusCode Render(HostListRef* p_pSettingsList)
    {
        StatusCode err = RenderGeneral(p_pSettingsList);
        if (err != errNone)
        {
            return err;
        }

        return errNone;
    }

private:
    void InitDefaults()
    {
        m_scene = -1;
    }

    StatusCode RenderGeneral(HostListRef* p_pSettingsList)
    {
        boost::function<pluginapi_create_t> factory;
        std::shared_ptr<VoukoderPro::IClient> vkdrpro;

        try
        {
            factory = VOUKODERPRO_CREATE_INSTANCE;
            vkdrpro = factory();

            if (!vkdrpro)
            {
                g_Log(logLevelError, "VoukoderPro Plugin :: Failed to create VoukoderPro instance. (1)");
                return errFail;
            }
        }
        catch (boost::system::system_error e)
        {
            g_Log(logLevelError, "VoukoderPro Plugin :: Failed to create VoukoderPro instance. (2)");
            return errFail;
        }

        if (vkdrpro->init() < 0)
        {
            g_Log(logLevelError, "VoukoderPro Plugin :: Failed to initialize VoukoderPro.");
            return errFail;
        }

        // vkdrpro_scenes
        {
            // Fetch all available scenes
            std::vector<std::shared_ptr<VoukoderPro::SceneInfo>> scenes;
            vkdrpro->sceneManager()->load(scenes);

            // Populate scenes
            std::vector<std::string> names;
            std::vector<std::int32_t> hashes;
            for (const auto& sceneInfo : scenes)
            {
                names.push_back(sceneInfo->name);
                hashes.push_back(hash(sceneInfo->name.c_str()));
            }

            // No scenes have been defined
            if (scenes.size() == 0)
            {
                names.push_back("Please create a scene first");
                hashes.push_back(0);
            }

            // Create scene item
            HostUIConfigEntryRef item("vkdrpro_scene");
            item.MakeComboBox("Scene", names, hashes, m_scene);
            item.SetDisabled(scenes.size() == 0);
            if (!item.IsSuccess() || !p_pSettingsList->Append(&item))
            {
                g_Log(logLevelError, "VoukoderPro Plugin :: Failed to create config field: vkdrpro_scenes");
                return errFail;
            }
        }

        {
            HostUIConfigEntryRef item("vkdrpro_markers");
            item.MakeMarkerColorSelector("Chapter Marker", "Color", m_MarkerColor);
            if (!item.IsSuccess() || !p_pSettingsList->Append(&item))
            {
                g_Log(logLevelError, "VoukoderPro Plugin :: Failed to populate encoder preset UI entry");
                assert(false);
                return errFail;
            }
        }

        // vkdrpro_configure
        {
            HostUIConfigEntryRef item("vkdrpro_configure");
            item.MakeButton("Open Scene Designer ...");
            item.SetTriggersUpdate(true);
            if (!item.IsSuccess() || !p_pSettingsList->Append(&item))
            {
                g_Log(logLevelError, "VoukoderPro Plugin :: Failed to create config button");
                return errFail;
            }
        }

        return errNone;
    }

public:
    int32_t GetNumPasses()
    {
        return 1;
    }

    const std::string& GetMarkerColor() const
    {
        return m_MarkerColor;
    }

private:
    HostCodecConfigCommon m_CommonProps;
    int32_t m_scene;
    std::string m_MarkerColor;
};

StatusCode VideoCodec::s_GetEncoderSettings(HostPropertyCollectionRef* p_pValues, HostListRef* p_pSettingsList)
{
    HostCodecConfigCommon commonProps;
    commonProps.Load(p_pValues);

    UISettingsController settings(commonProps);
    settings.Load(p_pValues);

    return settings.Render(p_pSettingsList);
}

StatusCode VideoCodec::s_RegisterCodecs(HostListRef* p_pList,
    const uint8_t* uuid,
    const char* pCodecName,
    uint32_t fourCC,
    uint32_t colorModel,
    uint8_t hSampling,
    uint8_t vSampling,
    uint32_t bitsPerSample)
{
    HostPropertyCollectionRef codecInfo;
    if (!codecInfo.IsValid())
        return errAlloc;

    codecInfo.SetProperty(pIOPropUUID, propTypeUInt8, uuid, 16);
    codecInfo.SetProperty(pIOPropName, propTypeString, pCodecName, (int)strlen(pCodecName));
    const char* pCodecGroup = "VoukoderPro";
    codecInfo.SetProperty(pIOPropGroup, propTypeString, pCodecGroup, (int)strlen(pCodecGroup));
    codecInfo.SetProperty(pIOPropFourCC, propTypeUInt32, &fourCC, 1);
    uint32_t val = mediaVideo;
    codecInfo.SetProperty(pIOPropMediaType, propTypeUInt32, &val, 1);
    val = dirEncode;
    codecInfo.SetProperty(pIOPropCodecDirection, propTypeUInt32, &val, 1);
    codecInfo.SetProperty(pIOPropColorModel, propTypeUInt32, &colorModel, 1);
    codecInfo.SetProperty(pIOPropHSubsampling, propTypeUInt8, &hSampling, 1);
    codecInfo.SetProperty(pIOPropVSubsampling, propTypeUInt8, &vSampling, 1);
    //codecInfo.SetProperty(pIOPropBitDepth, propTypeUInt32, &bitsPerSamle, 1);
    codecInfo.SetProperty(pIOPropBitsPerSample, propTypeUInt32, &bitsPerSample, 1);
    //uint8_t uval = 0;
    //codecInfo.SetProperty(pIOPropDataRange, propTypeUInt8, &uval, 1);
    //uval = 1;
    //codecInfo.SetProperty(pIOPropHasAlpha, propTypeUInt8, &uval, 1);

    const uint32_t temp = 0;
    codecInfo.SetProperty(pIOPropTemporalReordering, propTypeUInt32, &temp, 1);

    const uint8_t fieldSupport = (fieldProgressive | fieldTop | fieldBottom);
    codecInfo.SetProperty(pIOPropFieldOrder, propTypeUInt8, &fieldSupport, 1);

    std::vector<std::string> containerVec;
    containerVec.push_back("69fda3108d8e427fb03287b63d10e878");
    std::string valStrings;
    for (size_t i = 0; i < containerVec.size(); ++i)
    {
        valStrings.append(containerVec[i]);
        if (i < (containerVec.size() - 1))
            valStrings.append(1, '\0');
    }

    codecInfo.SetProperty(pIOPropContainerList, propTypeString, valStrings.c_str(), (int)valStrings.size());

    if (!p_pList->Append(&codecInfo))
        return errFail;

    return errNone;
}

VideoCodec::VideoCodec():
    m_Error(errNone)
{}

VideoCodec::~VideoCodec()
{}

void VideoCodec::DoFlush()
{
    if (m_Error != errNone)
        return;

    StatusCode sts = DoProcess(NULL);
    while (sts == errNone)
        sts = DoProcess(NULL);

    ++m_PassesDone;
}

StatusCode VideoCodec::DoInit(HostPropertyCollectionRef* p_pProps)
{
    _CRT_UNUSED(p_pProps);

    return errNone;
}

StatusCode VideoCodec::DoOpen(HostBufferRef* p_pBuff)
{
    m_CommonProps.Load(p_pBuff);
    m_pSettings.reset(new UISettingsController(m_CommonProps));
    m_pSettings->Load(p_pBuff);

    uint8_t isMultiPass = 0;
    _CRT_UNUSED(isMultiPass);
    if (m_pSettings->GetNumPasses() == 2)
    {
        g_Log(logLevelError, "Multi pass encoding is currently not supported.");
        return errInvalidParam;

        //m_IsMultiPass = true;
        //isMultiPass = 1;
    }

    //StatusCode sts = p_pBuff->SetProperty(pIOPropMultiPass, propTypeUInt8, &isMultiPass, 1);
    //if (sts != errNone)
    //{
    //    return sts;
    //}

    return errNone;
}

StatusCode VideoCodec::DoProcess(HostBufferRef* p_pBuff)
{
    if (m_Error != errNone)
        return m_Error;

    if ((p_pBuff == NULL || !p_pBuff->IsValid()))
        return errMoreData;

    int64_t pts = 0;
    if (!p_pBuff->GetINT64(pIOPropPTS, pts))
    {
        g_Log(logLevelError, "VoukoderPro Plugin :: PTS not set when encoding the frame.");
        return errNoParam;
    }

    p_pBuff->SetProperty(pIOPropDTS, propTypeInt64, &pts, 1);

    return m_pCallback->SendOutput(p_pBuff);
}
