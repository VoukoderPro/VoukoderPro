#pragma once

#include "../VoukoderPro/voukoderpro_api.h"
#include "nlohmann/json.hpp"

#include <stdexcept>
#include <algorithm>
#include <map>
#include <boost/config.hpp>
#include <boost/dll/alias.hpp>
#include <boost/describe/enum.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "properties.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
#include <libavutil/channel_layout.h>
#include <libavutil/cpu.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/mastering_display_metadata.h>
}

#pragma comment(lib, "avformat-voukoderpro.lib")
#pragma comment(lib, "avcodec-voukoderpro.lib")
#pragma comment(lib, "avfilter-voukoderpro.lib")
#pragma comment(lib, "avutil-voukoderpro.lib")
#pragma comment(lib, "swscale-voukoderpro.lib")
#pragma comment(lib, "swresample-voukoderpro.lib")

namespace VoukoderPro
{
    class BOOST_SYMBOL_VISIBLE Asset
    {
    public:
        Asset(const AssetInfo& info) :
            info(info)
        {}

        virtual int init(configType&)
        {
            return 0;
        }

        virtual int open(nlohmann::ordered_json& options)
        {
            return 0;
        }

        virtual int close()
        {
            return 0;
        }

    public:
        AssetInfo info;
    };

    class BOOST_SYMBOL_VISIBLE InputAsset : public Asset
    {
    public:
        InputAsset(const AssetInfo& info) : Asset(info)
        {}
    }; 
    
    class BOOST_SYMBOL_VISIBLE FilterAsset : public Asset
    {
    public:
        FilterAsset(const AssetInfo& info) : Asset(info)
        {}
    };

    class BOOST_SYMBOL_VISIBLE EncoderAsset : public Asset
    {
    public:
        EncoderAsset(const AssetInfo& info): Asset(info)
        {
            // Find the right encoder
            const AVCodec* codec = avcodec_find_encoder_by_name(info.id.c_str());
            if (codec)
                codecCtx = std::shared_ptr<AVCodecContext>(avcodec_alloc_context3(codec), [](AVCodecContext* ptr) { avcodec_free_context(&ptr); });
        }

        int init(configType& properties)
        {
            int ret = 0;

            // Configure the codec context
            switch (codecCtx->codec_type)
            {
            case AVMEDIA_TYPE_VIDEO:
            {
                codecCtx->thread_type = FF_THREAD_FRAME;
                codecCtx->thread_count = 0;
                codecCtx->width = std::get<int>(properties.at(pPropWidth));
                codecCtx->height = std::get<int>(properties.at(pPropHeight));
                codecCtx->time_base.num = std::get<int>(properties.at(pPropTimebaseNum));
                codecCtx->time_base.den = std::get<int>(properties.at(pPropTimebaseDen));
                codecCtx->framerate = av_inv_q(codecCtx->time_base);
                codecCtx->sample_aspect_ratio.num = std::get<int>(properties.at(pPropAspectNum));
                codecCtx->sample_aspect_ratio.den = std::get<int>(properties.at(pPropAspectDen));
                std::string fieldOrder = std::get<std::string>(properties.at(pPropFieldOrder));
                if (fieldOrder == "tff")
                    codecCtx->field_order = AV_FIELD_TT;
                else if (fieldOrder == "bff")
                    codecCtx->field_order = AV_FIELD_BB;
                else
                    codecCtx->field_order = AV_FIELD_PROGRESSIVE;
                codecCtx->pix_fmt = av_get_pix_fmt(std::get<std::string>(properties.at(VoukoderPro::pPropFormat)).c_str());
                if (properties.find(VoukoderPro::pPropColorRange) != properties.end())
                    codecCtx->color_range = (AVColorRange)av_color_range_from_name(std::get<std::string>(properties.at(VoukoderPro::pPropColorRange)).c_str());
                if (properties.find(VoukoderPro::pPropColorSpace) != properties.end())
                    codecCtx->colorspace = (AVColorSpace)av_color_space_from_name(std::get<std::string>(properties.at(VoukoderPro::pPropColorSpace)).c_str());
                if (properties.find(VoukoderPro::pPropColorPrimaries) != properties.end())
                    codecCtx->color_primaries = (AVColorPrimaries)av_color_primaries_from_name(std::get<std::string>(properties.at(VoukoderPro::pPropColorPrimaries)).c_str());
                if (properties.find(VoukoderPro::pPropColorTransfer) != properties.end())
                    codecCtx->color_trc = (AVColorTransferCharacteristic)av_color_transfer_from_name(std::get<std::string>(properties.at(VoukoderPro::pPropColorTransfer)).c_str());
                break;
            }
            case AVMEDIA_TYPE_AUDIO:
                codecCtx->sample_rate = std::get<int>(properties.at(pPropSamplingRate));
                codecCtx->time_base.num = 1;
                codecCtx->time_base.den = codecCtx->sample_rate;
                av_channel_layout_channel_from_string(&codecCtx->ch_layout, std::get<std::string>(properties.at(pPropChannelLayout)).c_str());
                codecCtx->sample_fmt = av_get_sample_fmt(std::get<std::string>(properties.at(pPropFormat)).c_str());
                break;

            default:
                ret = -1;
            }

            return ret;
        }

        int open(nlohmann::ordered_json& options)
        {
            AVDictionary* opts = nullptr;

            for (auto const& [key, val] : options.items())
            {
                std::string value = "";
                if (val.is_number_integer())
                    value = std::to_string(val.get<int>());
                else if (val.is_number())
                    value = std::to_string(val.get<double>());
                else if (val.is_string())
                    value = val.get<std::string>();
                else if (val.is_boolean())
                    value = val.get<bool>() ? "1" : "0";
                else
                    value += "error";

                av_dict_set(&opts, key.c_str(), value.c_str(), 0);
            }

            return avcodec_open2(codecCtx.get(), codecCtx->codec, &opts);
        }

        int close()
        {
            return avcodec_close(codecCtx.get());
        }

        void setCodecContext(std::shared_ptr<AVCodecContext> codecCtx)
        {
            this->codecCtx = codecCtx;
        }

        std::shared_ptr<AVCodecContext> getCodecContext()
        {
            return codecCtx;
        }

        int encode(std::shared_ptr<AVFrame> frame, std::function<int(std::shared_ptr<AVCodecContext>, std::shared_ptr<AVPacket>)> callback)
        {
            int ret = 0;

            auto receivePackets = [](std::shared_ptr<AVCodecContext> codecCtx, std::shared_ptr<AVPacket> packet, std::function<int(std::shared_ptr<AVCodecContext> codecCtx, std::shared_ptr<AVPacket>)> callback)
            {
                int ret = 0;

                while (ret >= 0)
                {
                    // Receive next packet
                    ret = avcodec_receive_packet(codecCtx.get(), packet.get());

                    // Did we get actual packets?
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                        return 0;
                    else if (ret < 0)
                        return ret;

                    // Return packets by callback
                    callback(codecCtx, packet);

                    av_packet_unref(packet.get());
                }

                return ret;
            };

            // Send the frame to the encoder
            if ((ret = avcodec_send_frame(codecCtx.get(), frame.get())) < 0)
            {
                // Is the send buffer full?
                if (ret == AVERROR(EAGAIN))
                {
                    // Read output buffer first
                    receivePackets(codecCtx, packet, callback);

                    // Retry sending the frame
                    return encode(frame, callback);
                }
            }

            // Check output buffer again
            if (ret == 0)
                ret = receivePackets(codecCtx, packet, callback);

            return ret;
        }

        bool isAvailable()
        {
            bool ret = false;

            auto codec = avcodec_find_encoder_by_name(info.id.c_str());
            if (codec != NULL)
            {
                AVCodecContext* codecContext = avcodec_alloc_context3(codec);
                if (codecContext != NULL)
                {
                    codecContext->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

                    if (codec->type == AVMEDIA_TYPE_VIDEO)
                    {
                        codecContext->width = 320;
                        codecContext->height = 240;
                        codecContext->time_base = { 1, 25 };
                        codecContext->framerate = av_inv_q(codecContext->time_base);
                        codecContext->sample_aspect_ratio = { 1, 1 };
                        codecContext->field_order = AV_FIELD_PROGRESSIVE;
                        codecContext->pix_fmt = codec->pix_fmts ? codec->pix_fmts[0] : AV_PIX_FMT_YUV420P;
                    }
                    else if (codec->type == AVMEDIA_TYPE_AUDIO)
                    {
                        //if (codec->ch_layouts)
                        //    codecContext->ch_layout = codec->ch_layouts[0];
                        //else
                        //    av_channel_layout_from_mask(&codecContext->ch_layout, AV_CH_LAYOUT_STEREO);
                        //codecContext->sample_rate = codec->supported_samplerates ? codec->supported_samplerates[0] : 48000;
                        //codecContext->sample_fmt = codec->sample_fmts ? codec->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
                    }
                    else
                    {
                        return false;
                    }

                    // Open the codec
                    try
                    {
                        const int res = avcodec_open2(codecContext, codec, NULL);

                        // Only 0 is successful
                        ret = res == 0;

                        //if (!ret)
                        //    vkLogInfoVA("- Encoder initialization failed (Code: %d)", res);
                    }
                    catch (...)
                    {
                        return false;
                    }

                    // Close the codec
                    avcodec_free_context(&codecContext);
                }
            }

            return ret;
        }

    private:
        std::shared_ptr<AVCodecContext> codecCtx = nullptr;
        std::shared_ptr<AVPacket> packet = std::shared_ptr<AVPacket>(av_packet_alloc(), [](AVPacket* ptr) { av_packet_free(&ptr); });
    };

    class BOOST_SYMBOL_VISIBLE MuxerAsset : public Asset
    {
    public:
        MuxerAsset(const AssetInfo& info) : Asset(info)
        {}
    };

    class BOOST_SYMBOL_VISIBLE OutputAsset : public Asset
    {
    public:
        OutputAsset(const AssetInfo& info) : Asset(info)
        {}
    };

    class BOOST_SYMBOL_VISIBLE PostProcAsset : public Asset
    {
    public:
        PostProcAsset(const AssetInfo& info) : Asset(info)
        {}
    };

    class BOOST_SYMBOL_VISIBLE plugin_api
    {
    public:
        std::vector<AssetInfo> infos;

        plugin_api()
        {}

        virtual ~plugin_api() {}

        virtual std::shared_ptr<Asset> createAsset(const AssetInfo& info)
        {
            return std::make_shared<Asset>(info);
        }

        virtual int registerAsset(AssetInfo& info)
        {
            infos.push_back(info);

            return 0;
        }

    protected:
        /**
         * Try to make a useful name out of an id.
         */
        static const std::string Id2Name(const std::string id)
        {
            std::string name(id);
            name[0] = std::toupper(name[0]);

            std::replace(name.begin(), name.end(), '_', ' ');

            return name;
        }

        /**
         * Create the parameter list based on the info stored in FFmpeg.
         */
        const void createFFmpegParameters(AssetInfo& info, const AVClass* priv_class)
        {
            const auto dbl2int = [](const double value) -> int {
                int ret;
                if (value > (double)INT_MAX)
                    ret = INT_MAX;
                else if (value < (double)INT_MIN)
                    ret = INT_MIN;
                else if (std::isnan(value))
                    ret = 0;
                else
                    ret = static_cast<int>(value);
                return ret;
            };

            auto& basic = info.group("param", "Parameters", ItemParamGroupType::Standard);

            int prevOffset = -1;
            const AVOption* option = NULL;
            while ((option = av_opt_next(&priv_class, option)) != NULL)
            {
                // Skip option values
                if (option->type == AV_OPT_TYPE_CONST || prevOffset == option->offset)
                    continue;

                prevOffset = option->offset;

                const std::string name = Id2Name(option->name);
                const std::string description = option->help ? Id2Name(option->help) : "";

                //
                std::string type;
                switch (option->type)
                {
                case AV_OPT_TYPE_INT:
                case AV_OPT_TYPE_INT64:
                case AV_OPT_TYPE_UINT64:
                {
                    ItemParam<int>& param = basic.param<int>(option->name, name)
                        .description(description)
                        .defaultValue(dbl2int((double)option->default_val.i64));

                    if (option->unit)
                    {
                        int64_t po = INT_MIN;
                        const AVOption* unit = NULL;

                        while ((unit = av_opt_next(&priv_class, unit)))
                            if (unit->type == AV_OPT_TYPE_CONST && unit->unit && !strcmp(unit->unit, option->unit) && po != unit->default_val.i64)
                            {
                                po = unit->default_val.i64;
                                param.option(Id2Name(unit->help && strlen(unit->help) > 0 ? unit->help : unit->name), (int)unit->default_val.i64);
                            }
                    }
                    else
                    {
                        param.minValue(dbl2int(option->min));
                        param.maxValue(dbl2int(option->max));
                    }
                    break;
                }

                case AV_OPT_TYPE_DOUBLE:
                case AV_OPT_TYPE_FLOAT:
                {
                    ItemParam<double>& param = basic.param<double>(option->name, name)
                        .description(description)
                        .defaultValue(std::isnan(option->default_val.dbl) ? std::min(std::max(option->min, 0.0), option->max) : option->default_val.dbl);

                    if (!option->unit)
                    {
                        param.minValue(option->min);
                        param.maxValue(option->max);
                    }
                    break;
                }

                case AV_OPT_TYPE_BOOL:
                {
                    basic.param<bool>(option->name, name)
                        .description(description)
                        .defaultValue(option->default_val.i64 > 0);
                    break;
                }

                case AV_OPT_TYPE_STRING:
                case AV_OPT_TYPE_COLOR:
                case AV_OPT_TYPE_IMAGE_SIZE:
                case AV_OPT_TYPE_VIDEO_RATE:
                {
                    basic.param<std::string>(option->name, name)
                        .description(description)
                        .defaultValue(option->default_val.str ? option->default_val.str : "");
                    break;
                }

                case AV_OPT_TYPE_RATIONAL:
                {
                    ItemParam<std::string> item = basic.param<std::string>(option->name, name)
                        .description(description);

                    if (option->default_val.dbl)
                    {
                        const AVRational ratio = av_d2q(option->default_val.dbl, INT_MAX);
                        item.defaultValue(std::to_string(ratio.num) + "/" + std::to_string(ratio.den));
                    }
                    break;
                }

                case AV_OPT_TYPE_FLAGS:
                    basic.param<flags>(option->name, name)
                        .description(description)
                        .defaultValue(0);
                    break;

                case AV_OPT_TYPE_DURATION:
                    basic.param<std::string>(option->name, name)
                        .description(description)
                        .defaultValue(std::to_string(option->default_val.i64) + "s");
                    break;

                default:
                    continue;
                }
            }
        }
    };

    class BOOST_SYMBOL_VISIBLE InputPlugin : public plugin_api
    {
    public:
        InputPlugin() : plugin_api()
        {}

        virtual std::shared_ptr<Asset> createAsset(const AssetInfo& info)
        {
            return std::make_shared<InputAsset>(info);
        }
    };

    class BOOST_SYMBOL_VISIBLE FilterPlugin : public plugin_api
    {
    public:
        FilterPlugin() : plugin_api()
        {}

        virtual std::shared_ptr<Asset> createAsset(const AssetInfo& info)
        {
            return std::make_shared<FilterAsset>(info);
        }
    };

    class BOOST_SYMBOL_VISIBLE EncoderPlugin : public plugin_api
    {
    public:
        EncoderPlugin(): plugin_api()
        {}

        virtual std::shared_ptr<Asset> createAsset(const AssetInfo& info)
        {
            return std::make_shared<EncoderAsset>(info);
        }

    protected:
        int registerAsset(AssetInfo& info)
        {
            // Find encoder by name
            const AVCodec* codec = avcodec_find_encoder_by_name(info.id.c_str());
            if (!codec)
                return -1;

            info.codec = codec->id;

            return plugin_api::registerAsset(info);
        }
    };

    class BOOST_SYMBOL_VISIBLE MuxerPlugin : public plugin_api
    {
    public:
        MuxerPlugin() : plugin_api()
        {}

        virtual std::shared_ptr<Asset> createAsset(const AssetInfo& info)
        {
            return std::make_shared<MuxerAsset>(info);
        }
    };

    class BOOST_SYMBOL_VISIBLE OutputPlugin : public plugin_api
    {
    public:
        OutputPlugin() : plugin_api()
        {}

        virtual std::shared_ptr<Asset> createAsset(const AssetInfo& info)
        {
            return std::make_shared<OutputAsset>(info);
        }
    };

    class BOOST_SYMBOL_VISIBLE PostProcPlugin : public plugin_api
    {
    public:
        PostProcPlugin() : plugin_api()
        {}

        virtual std::shared_ptr<Asset> createAsset(const AssetInfo& info)
        {
            return std::make_shared<PostProcAsset>(info);
        }
    };
}
