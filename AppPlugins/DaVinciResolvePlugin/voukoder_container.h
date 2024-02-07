#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>

#include "json.hpp"
#include "ioplugin_sdk/plugin_api.h"
#include "video_encoder.h"
#include "voukoderpro.hpp"

using json = nlohmann::json;
using namespace IOPlugin;

class DummyTrackWriter;

class VoukoderFormat : public IPluginContainerRef, public VoukoderBase
{
public:
    static const uint8_t s_UUID[];

public:
    VoukoderFormat();
    static StatusCode s_Register(HostListRef* p_pList);
    StatusCode WriteVideo(uint32_t p_TrackIdx, HostBufferRef* p_pBuf);
    StatusCode WriteAudio(uint32_t p_TrackIdx, HostBufferRef* p_pBuf);

protected:
    virtual StatusCode DoInit(HostPropertyCollectionRef* p_pProps);
    virtual StatusCode DoOpen(HostPropertyCollectionRef* p_pProps);
    virtual StatusCode DoAddTrack(HostPropertyCollectionRef* p_pProps, HostPropertyCollectionRef* p_pCodecProps, IPluginTrackBase** p_pTrack);
    virtual StatusCode DoClose();

protected:
    virtual ~VoukoderFormat();
    std::vector<DummyTrackWriter*> m_TrackVec;
    VoukoderPro::config project;

private:
    long long GetFrames(double seconds, double n, double d);
    std::string GetTimecode(double seconds, double n, double d);
};
