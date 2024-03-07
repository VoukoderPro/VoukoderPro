#pragma once

#include <functional>

#include "../../PluginInterface/plugin_api.h"

namespace VoukoderPro
{
	class FFmpegEncodersPlugin : public EncoderPlugin
	{
	public:
		FFmpegEncodersPlugin();

	public:
		static std::shared_ptr<FFmpegEncodersPlugin> CreateInstance()
		{
			return std::make_shared<FFmpegEncodersPlugin>();
		}

	private:
		int registerAAC();
		int registerAC3();
		int registerALAC();
		int registerCFHD();
		int registerDCA();
		int registerEAC3();
		int registerFFv1();
		int registerFFvhuff();
		int registerFLV1();
		int registerFLAC();
		int registerGIF();
		int registerVP9();
		int registerHAP();
		int registerMP2();
		int registerMPEG2Video();
		int registerPCM16le();
		int registerPCM24le();
		int registerPCM32le();
		int registerProResKS();
		int registerTrueHD();
		int registerUTVideo();
		int registerQuickTimeRLE();
		int registerWavPack();
	};

	BOOST_DLL_ALIAS(VoukoderPro::FFmpegEncodersPlugin::CreateInstance, CreateInstance)
}
