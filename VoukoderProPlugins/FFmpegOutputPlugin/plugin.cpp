#include "plugin.h"

#include <filesystem>

/*
  crypto
  fd
  ffrtmphttp
  file
  ftp
  gopher
  gophers
  http
  httpproxy
  https
  icecast
  md5
  pipe
  prompeg
  rtmp
  rtmps
  rtmpt
  rtmpts
  rtp
  srtp
  tee
  tcp
  tls
  udp
  udplite
  */

namespace VoukoderPro
{
    FFmpegOutputsPlugin::FFmpegOutputsPlugin()
    {
        // FILE
        {
            AssetInfo info;
            info.id = "file";
            info.name = "File";
            info.category = std::make_pair("ffmpeg", "FFmpeg");
            info.description = "Write a file to the local disk.";
            info.type = NodeInfoType::output;
            info.mediaType = MediaType::mux;
            info.helpUrl = "https://ffmpeg.org/ffmpeg-protocols.html#" + info.id;
            registerAsset(info);
        }

        // FTP
        {
            AssetInfo info;
            info.id = "ftp";
            info.name = "FTP";
            info.category = std::make_pair("ffmpeg", "FFmpeg");
            info.description = "Upload a file to an FTP server.";
            info.type = NodeInfoType::output;
            info.mediaType = MediaType::mux;
            info.helpUrl = "https://ffmpeg.org/ffmpeg-protocols.html#" + info.id;
            registerAsset(info);
        }
    }
}
