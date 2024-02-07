#pragma once

#include <memory>
#include <boost/function.hpp>
#include <boost/dll.hpp>

#include "ioplugin_sdk/plugin_api.h"
#include "../../VoukoderPro/voukoderpro_api.h"

using namespace IOPlugin;

typedef boost::shared_ptr<VoukoderPro::IClient>(pluginapi_create_t)();

class UISettingsController;

static inline int hash(const char* str)
{
    int h = 0;
    while (*str)
        h = h << 1 ^ *str++;
    return h;
}

class VideoCodec: public IPluginCodecRef
{
public:
    static const uint8_t s_UUID_420_8[];
    static const uint8_t s_UUID_422_8[];
    static const uint8_t s_UUID_444_8[];
    static const uint8_t s_UUID_422_10[];
    static const uint8_t s_UUID_444_10[];
    static const uint8_t s_UUID_444_16[];
    static const uint8_t s_UUID_RGBA_8[];
    static const uint8_t s_UUID_RGBA_16[];
    static const uint8_t s_UUID_AYUV_16[];

public:
    VideoCodec();
    ~VideoCodec();

    static StatusCode s_RegisterCodecs(HostListRef* p_pList,
        const uint8_t* uuid,
        const char* pCodecName,
        uint32_t fourCC,
        uint32_t colorModel,
        uint8_t hSampling,
        uint8_t vSampling,
        uint32_t bitsPerSamle);
    static StatusCode s_GetEncoderSettings(HostPropertyCollectionRef* p_pValues, HostListRef* p_pSettingsList);

    virtual bool IsNeedNextPass() override
    {
        return (m_IsMultiPass && (m_PassesDone < 2));
    }

    virtual bool IsAcceptingFrame(int64_t p_PTS) override
    {
        _CRT_UNUSED(p_PTS);
        return (m_IsMultiPass && (m_PassesDone < 3));
    }

protected:
    virtual void DoFlush() override;
    virtual StatusCode DoInit(HostPropertyCollectionRef* p_pProps) override;
    virtual StatusCode DoOpen(HostBufferRef* p_pBuff) override;
    virtual StatusCode DoProcess(HostBufferRef* p_pBuff) override;

private:
    std::unique_ptr<UISettingsController> m_pSettings;
    HostCodecConfigCommon m_CommonProps;
    bool m_IsMultiPass;
    uint32_t m_PassesDone;
    StatusCode m_Error;
};
