#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class FFmpegMuxerPlugin: public MuxerPlugin
	{
	public:
		FFmpegMuxerPlugin();

	public:
		static std::shared_ptr<FFmpegMuxerPlugin> CreateInstance()
		{
			return std::make_shared<FFmpegMuxerPlugin>();
		}

	private:
		void registerAAC();
		void registerAC3();
		void registerAVI();
		void registerFLV();
		void registerGIF();
		void registerMatroska();
		void registerMOV();
		void registerMP2();
		void registerMP3();
		void registerMP4();
		void registerMPEG2Video();
		void registerWEBM();
		void registerWAV();
		void registerWavpack();
	};

	BOOST_DLL_ALIAS(VoukoderPro::FFmpegMuxerPlugin::CreateInstance, CreateInstance)
}
