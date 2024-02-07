#include "plugin.h"

namespace VoukoderPro
{
    X264EncoderPlugin::X264EncoderPlugin():
        EncoderPlugin()
    {
        info.id = "9a2496d4-5763-4f89-9c2e-f90f53a2df7c";
        info.name = "x264";
        info.codec = std::make_pair("h264", "H.264 / AVC");
        info.description = "x264";
        info.type = NodeInfoType::encoder;
        info.mediaType = MediaType::video;

        codecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    }

    int X264EncoderPlugin::init(std::map<std::string, std::string>& properties)
    {
        // Call standard implementation
        if (EncoderPlugin::init(properties) < 0)
            return 0;

        // https://stackoverflow.com/questions/49181122/read-child-process-stdout-in-a-separate-thread-with-boost-process
        // https://www.boost.org/doc/libs/1_79_0/doc/html/boost_process/tutorial.html
        // http://forum.doom9.org/archive/index.php/t-161260.html


        std::future<std::string> data;

        x264 = std::make_unique<bp::child>("C:\\Users\\Daniel\\source\\repos\\VoukoderPro\\x64\\Debug\\plugins\\x264.exe", "", 
            bp::std_in.close(),
            bp::std_out > data, //so it can be written without anything
            bp::std_err > data,
            ioservice);
        




        auto err = data.get();
        return 0;
    }

    int X264EncoderPlugin::open(std::map<std::string, std::string> options)
    {
        ioservice.run();

        return 0;
    }

    int X264EncoderPlugin::close()
    {
        // Call standard implementation
        if (EncoderPlugin::close() < 0)
            return 0;

        return 0;
    }

    int X264EncoderPlugin::encode(std::shared_ptr<AVFrame> frame, std::function<int(std::shared_ptr<AVPacket>)> callback)
    {
        return 0;

    }
}