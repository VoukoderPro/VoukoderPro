#pragma once

#include <functional>
#include <boost/process.hpp>
#include <boost/asio/io_service.hpp>

#include "../../PluginInterface/plugin_api.h"

namespace bp = boost::process;

namespace VoukoderPro
{
	class X264EncoderPlugin : public EncoderPlugin
	{
	public:
		X264EncoderPlugin();

		int init(std::map<std::string, std::string>& properties);
        int open(std::map<std::string, std::string> options = {});
		int close();
		int encode(std::shared_ptr<AVFrame> frame, std::function<int(std::shared_ptr<AVPacket>)> callback);

	private:
		std::unique_ptr<bp::child> x264 = nullptr;
		boost::asio::io_service ioservice;

	public:
		static std::shared_ptr<X264EncoderPlugin> CreateInstance()
		{
			return std::make_shared<X264EncoderPlugin>();
		}
	};

	BOOST_DLL_ALIAS(VoukoderPro::X264EncoderPlugin::CreateInstance, CreateInstance)
}
