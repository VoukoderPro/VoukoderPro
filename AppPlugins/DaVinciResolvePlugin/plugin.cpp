#include "plugin.h"

#include <assert.h>
#include <cstring>

#include "utils.h"
#include "video_encoder.h"
#include "voukoder_container.h"

static const uint8_t pMyUUID[] = { 0x56, 0x36, 0xe2, 0xf1, 0x88, 0x63, 0x4b, 0x6c, 0xa7, 0x49, 0xb7, 0x7e, 0xdf, 0x16, 0x3a, 0xdb };

using namespace IOPlugin;

StatusCode g_HandleGetInfo(HostPropertyCollectionRef* p_pProps)
{
    StatusCode err = p_pProps->SetProperty(pIOPropUUID, propTypeUInt8, pMyUUID, 16);
    if (err == errNone)
    {
        std::string plugin = "VoukoderPro Plugin Version ";
        plugin += PLUGIN_VERSION;

        err = p_pProps->SetProperty(pIOPropName, propTypeString, plugin.c_str(), static_cast<int>(plugin.length()));
        g_Log(logLevelInfo, plugin.c_str());
    }

    return err;
}

StatusCode g_HandleCreateObj(unsigned char* p_pUUID, ObjectRef* p_ppObj)
{
    if (memcmp(p_pUUID, VideoCodec::s_UUID_420_8, 16) == 0 ||
        memcmp(p_pUUID, VideoCodec::s_UUID_422_8, 16) == 0 ||
        memcmp(p_pUUID, VideoCodec::s_UUID_444_8, 16) == 0 ||
        //memcmp(p_pUUID, VideoCodec::s_UUID_422_10, 16) == 0 ||
        memcmp(p_pUUID, VideoCodec::s_UUID_444_16, 16) == 0 ||
        memcmp(p_pUUID, VideoCodec::s_UUID_RGBA_8, 16) == 0 ||
        memcmp(p_pUUID, VideoCodec::s_UUID_AYUV_16, 16) == 0)
    {
        *p_ppObj = new VideoCodec();
        return errNone;
    }
    else if (memcmp(p_pUUID, VoukoderFormat::s_UUID, 16) == 0)
    {
        *p_ppObj = new VoukoderFormat();
        return errNone;
    }

    return errUnsupported;
}

StatusCode g_HandlePluginStart()
{
    boost::function<pluginapi_create_t> factory;
    std::shared_ptr<VoukoderPro::IClient> vkdrpro;

    try
    {
        factory = VoukoderProCreateInstance();
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

    vkdrpro->event("startup", {
        { "dimension1", "DaVinci Resolve Studio" },
        { "dimension2", "DaVinci Resolve Studio " + GetAppVersionFromFile() }
    });

    return errNone;
}

StatusCode g_HandlePluginTerminate()
{
    return errNone;
}

StatusCode g_ListCodecs(HostListRef* p_pList)
{
    // Video
    VideoCodec::s_RegisterCodecs(p_pList, VideoCodec::s_UUID_420_8, "YUV 4:2:0 (8 bit)", 'vkp0', clrYUVp, 2, 2, 8);
    VideoCodec::s_RegisterCodecs(p_pList, VideoCodec::s_UUID_422_8, "YUV 4:2:2 (8 bit)", 'vkp1', clrUYVY, 2, 1, 8);
    VideoCodec::s_RegisterCodecs(p_pList, VideoCodec::s_UUID_444_8, "YUV 4:4:4 (8 bit)", 'vkp2', clrYUVp, 1, 1, 8);
    //VideoCodec::s_RegisterCodecs(p_pList, VideoCodec::s_UUID_422_10, "YUV 4:2:2 (10 bit)", 'vkp3', clrV210, 2, 1, 10);
    VideoCodec::s_RegisterCodecs(p_pList, VideoCodec::s_UUID_444_16, "YUV 4:4:4 (16 bit)", 'vkp4', clrYUVp, 1, 1, 16);
    VideoCodec::s_RegisterCodecs(p_pList, VideoCodec::s_UUID_AYUV_16, "YUVA 4:4:4 (16 bit, Alpha)", 'vkp5', clrAYUV, 1, 1, 16);

    // Audio

    return errNone;
}

StatusCode g_ListContainers(HostListRef* p_pList)
{
    return VoukoderFormat::s_Register(p_pList);
}

StatusCode g_GetEncoderSettings(unsigned char* p_pUUID, HostPropertyCollectionRef* p_pValues, HostListRef* p_pSettingsList)
{
    return VideoCodec::s_GetEncoderSettings(p_pValues, p_pSettingsList);
}
