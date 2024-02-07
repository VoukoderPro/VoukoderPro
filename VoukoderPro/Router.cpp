#include "Router.h"

#include <iostream>

#include "Assets.h"
#include "json.hpp"
#include "Errors.h"

using json = nlohmann::json;

namespace VoukoderPro
{
    Router::Router(std::shared_ptr<Scene> scene, config project) :
        scene(scene), project(project)
    {}

    /**
    * 
    */
    int Router::init()
    {
        sysinfo();

        int ret = ERR_OK;

        // Process each nle track coming from the NLE and find an input node for it
        for (int nleTrackIndex = 0; nleTrackIndex < project.tracks.size(); nleTrackIndex++)
        {
            const auto& nleTrack = project.tracks.at(nleTrackIndex);

            // We support audio and video input tracks only
            const std::string nleTrackType = std::get<std::string>(nleTrack.at(VoukoderPro::pPropType));
            MediaType mediaType;
            if (nleTrackType == "video")
                mediaType = MediaType::video;
            else if (nleTrackType == "audio")
                mediaType = MediaType::audio;
            else
            {
                BLOG(warning) << "NLE track media type \"" << nleTrackType << "\" is not supported! Proceeding with next track!";
                continue;
            }

            // Try to find the right VoukoderPro scene tracks
            std::set<std::shared_ptr<InputNode>> nleTrackInputNodes;
            if (findInputNodes(nleTrackInputNodes, nleTrackIndex, mediaType) == 0)
            {
                BLOG(info) << "NLE Track #" << nleTrackIndex << " is not used in this scene.";
                continue;
            }

            // Initialize the input nodes
            for (const auto& trackInputNode : nleTrackInputNodes)
            {
                // It's already in use for one source track
                if (!trackInputNode->nodeData(nleTrackIndex))
                {
                    // Build node data
                    auto data = std::make_shared<NodeData>();
                    data->properties.insert(project.begin(), project.end());
                    data->trackProperties.insert(nleTrack.begin(), nleTrack.end());
                    data->performance = std::make_unique<Performance>("track[type=" + nleTrackType + "]");

                    // Assign track to node
                    trackInputNode->setNodeData(nleTrackIndex, data);
                }

                // Register the nle track to this input node
                nleTrackToInputNodes.insert(std::make_pair(nleTrackIndex, nleTrackInputNodes));

                inputNodes.insert(trackInputNode);
            }
        }

        // Initialize each input node
        for (const auto& inputNode : inputNodes)
        {
            BLOG(info) << "### PHASE: PRE-INIT";

            // Pre-Init the node
            if ((ret = inputNode->preinit()) < ERR_OK)
            {
                BLOG(error) << "Pre-init phase for " << inputNode->nodeInfo->id << " failed!";

                break;
            }

            BLOG(info) << "### PHASE: INIT";

            // Init the node
            if ((ret = inputNode->init()) < ERR_OK)
            {
                BLOG(error) << "Init phase of " << inputNode->nodeInfo->id << " failed!";

                break;
            }
        }

        // Check if some outputs want to write to the same filename
        std::vector<std::string> filenames;
        for (const auto& pair : scene->nodes)
        {
            const std::shared_ptr<BaseNode> node = pair.second;

            // We only want output nodes
            if (node->nodeInfo->type == NodeInfoType::output)
            {
                std::string filename = node->nodeInfo->data[pPropOutputUrl].get<std::string>();

#ifdef _WIN32
                // Windows filenames are case insensitive
                std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
#endif

                if (std::find(filenames.begin(), filenames.end(), filename) != filenames.end())
                {
                    BLOG(error) << "Output filename '" + filename + "' is already assigned!";

                    ret = ERR_FILENAME_IN_USE;

                    break;
                }

                filenames.push_back(filename);
            }
        }

        if (ret < ERR_OK)
            BLOG(error) << "Initialization failed: " << ret;
        else
            BLOG(info) << "Initialization succeeded.";

        return ret;
    }

    /**
    * Execute preopen() and open() of all inputs.
    */
    int Router::open()
    {
        int ret = ERR_OK;

        BLOG(info) << "### PHASE: PRE-OPEN";

        // Pre-Init the nodes
        for (auto& inputNode : inputNodes)
            if ((ret = inputNode->preopen()) < ERR_OK)
                break;

        if (ret < ERR_OK)
            BLOG(error) << "Pre-open phase failed: " << ret;
        else
        {
            BLOG(info) << "Pre-open phase succeeded.";
            BLOG(info) << "### PHASE: OPEN";

            // Init the nodes
            for (auto& inputNode : inputNodes)
                if ((ret = inputNode->open()) < ERR_OK)
                {
                    BLOG(error) << "Open phase failed: " << ret;
                    break;
                }
        }

        if (ret == ERR_OK)
        {
            BLOG(info) << "Open phase succeeded.";
            BLOG(info) << "Waiting for frames ...";
        }
        
        return ret;
    }

    /**
    * Execute preclose() and close() of each input node.
    */
    int Router::close()
    {
        int ret = ERR_OK;

        BLOG(info) << "Executing pre-close phase ...";

        // Pre-close the node
        for (auto& inputNode : inputNodes)
            if ((ret = inputNode->preclose()) < ERR_OK)
                break;

        if (ret < ERR_OK)
            BLOG(error) << "Pre-close phase failed: " << ret;
        else
        {
            BLOG(info) << "Pre-close phase succeeded.";
            BLOG(info) << "Executing close phase ...";

            // Close the node
            for (auto& inputNode : inputNodes)
                if ((ret = inputNode->close()) < ERR_OK)
                {
                    BLOG(error) << "Close phase failed: " << ret;
                    break;
                }
        }

        nleTrackToInputNodes.clear();
        inputNodes.clear();

        if (ret == ERR_OK)
            BLOG(info) << "Close phase succeeded.";

        return ret;
    }

    /**
    * Sends an AVFrame to the input and the supplied track index.
    */
    int Router::sendFrame(const int nleTrackIndex, std::shared_ptr<AVFrame> frame)
    {
        int ret = ERR_OK;

        // Does this track exist?
        if (nleTrackToInputNodes.find(nleTrackIndex) != nleTrackToInputNodes.end())
        {
            // Performance log
            const auto inputNodes = nleTrackToInputNodes.at(nleTrackIndex);

            // Distribute frames
            for (const auto& inputNode : inputNodes)
                if ((ret = inputNode->sendFrame(nleTrackIndex, frame)) < ERR_OK)
                    break;
        }

        return ret;
    }

    /**
    * 
    */
    int Router::getPerformanceLog(nlohmann::ordered_json& log)
    {
        log["inputs"] = nlohmann::ordered_json::array();
        //log["scene"] = scene->info;

        // Append data per input track
        //for (const auto& [idx, input] : nleTrackToInputNodes)
        //    log["inputs"].push_back(input->nodeData()->performance->json());

        return ERR_OK;
    }

    /**
    */
    int Router::translateTrackIndex(const int search, MediaType mediaType)
    {
        //
        int typeIndex = 0, index = 0;
        for (const auto& track : project.tracks)
        {
            const std::string nleTrackType = std::get<std::string>(track.at(VoukoderPro::pPropType));
            if (((nleTrackType == "video" && mediaType == MediaType::video) ||
                (nleTrackType == "audio" && mediaType == MediaType::audio)))
            {
                if (typeIndex == search)
                    return index;

                typeIndex++;
            }

            index++;
        }

        return -1;
    }

    /**
    * Finds the VoukoderPro scene track for a given nle track.
    */
    int Router::findInputNodes(std::set<std::shared_ptr<InputNode>>& inputNodes, const int nleTrackIndex, const MediaType mediaType)
    {
        //
        for (const auto& [nodeId, node] : scene->nodes)
        {
            // Only deal with input tracks of the same media type
            if (node->nodeInfo->type == NodeInfoType::input && node->nodeInfo->mediaType == mediaType)
            {
                auto inputNode = std::static_pointer_cast<InputNode>(node);

                if (node->nodeInfo->data.contains("tracks") && node->nodeInfo->data["tracks"].is_array())
                {
                    const auto& tracks = node->nodeInfo->data["tracks"];

                    // No track numbers are specified -> the first track matches all nle tracks
                    if (tracks.size() == 0)
                        inputNodes.insert(inputNode);
                    else
                    {
                        // A track number is specified ...
                        for (const auto& track : tracks)
                            if (nleTrackIndex == translateTrackIndex(track.get<int>(), mediaType))
                            {
                                inputNodes.insert(inputNode);
                                break;
                            }
                    }
                }
                else // Fallback if no tracks-array is present
                    inputNodes.insert(inputNode);
            }
        }

        return (int)inputNodes.size();
    }

    void Router::sysinfo()
    {
        std::stringstream system;

        {
            system << "Dumping system information:\n\n"
                << "  Architecture: " << architecture_name(iware::cpu::architecture()) << '\n'
                << "  Vendor ID: " << iware::cpu::vendor_id() << '\n'
                << "  Model name: " << iware::cpu::model_name() << '\n'
                << "  Frequency: " << (iware::cpu::frequency() / 1000000) << " MHz\n";
        }

        {
            const auto quantities = iware::cpu::quantities();
            system << "\n"
                "  Quantities:\n"
                << "    CPU packages : " << quantities.packages << '\n'
                << "    Physical CPUs: " << quantities.physical << '\n'
                << "    Logical CPUs : " << quantities.logical << '\n';
        }

        {
            system << "\n"
                "  Caches:\n";
            for (auto i = 1u; i <= 3; ++i) {
                const auto cache = iware::cpu::cache(i);
                system << "    L" << i << ":\n"
                    << "      Size         : " << cache.size << "\n"
                    << "      Line size    : " << cache.line_size << "\n"
                    << "      Associativity: " << static_cast<unsigned int>(cache.associativity) << '\n'
                    << "      Type         : " << cache_type_name(cache.type) << '\n';
            }
        }

        {
            system << std::boolalpha
                << "\n"
                "  Instruction set support:\n";
            for (auto&& set : { std::make_pair("3D-now!", iware::cpu::instruction_set_t::s3d_now),  //
                              std::make_pair("MMX    ", iware::cpu::instruction_set_t::mmx),      //
                              std::make_pair("SSE    ", iware::cpu::instruction_set_t::sse),      //
                              std::make_pair("SSE2   ", iware::cpu::instruction_set_t::sse2),     //
                              std::make_pair("SSE3   ", iware::cpu::instruction_set_t::sse3),     //
                              std::make_pair("AVX    ", iware::cpu::instruction_set_t::avx) })
                system << "    " << set.first << ": " << iware::cpu::instruction_set_supported(set.second) << '\n';
        }

        {
            const auto memory = iware::system::memory();
            system << "\n"
                "  Memory:\n"
                "    Physical:\n"
                << "      Available: " << memory.physical_available << "\n"
                << "      Total    : " << memory.physical_total << "\n"
                << "    Virtual:\n"
                << "      Available: " << memory.virtual_available << "\n"
                << "      Total    : " << memory.virtual_total << "\n";
        }

        {
            const auto kernel_info = iware::system::kernel_info();
            system << "\n"
                "  Kernel:\n"
                << "    Variant: " << kernel_variant_name(kernel_info.variant) << '\n'
                << "    Version: " << kernel_info.major << '.' << kernel_info.minor << '.' << kernel_info.patch << " build " << kernel_info.build_number << '\n';
        }

        {
            const auto OS_info = iware::system::OS_info();
            system << "\n"
                "  OS:\n"
                << "    Name     : " << OS_info.name << '\n'
                << "    Full name: " << OS_info.full_name << '\n'
                << "    Version  : " << OS_info.major << '.' << OS_info.minor << '.' << OS_info.patch << " build " << OS_info.build_number << '\n';
        }

        {
            const auto device_properties = iware::gpu::device_properties();
            system << "\n"
                "  GPUs:\n";
            if (device_properties.empty())
                system << "    No detection methods enabled\n";
            else
                for (auto i = 0u; i < device_properties.size(); ++i) {
                    const auto& properties_of_device = device_properties[i];
                    system << "    Device #" << (i + 1) << ":\n"
                        << "      Vendor       : " << vendor_name(properties_of_device.vendor) << '\n'
                        << "      Name         : " << properties_of_device.name << '\n'
                        << "      RAM size     : " << properties_of_device.memory_size << "\n"
                        << "      Cache size   : " << properties_of_device.cache_size << "\n";
                }
        }

        BLOG(info) << system.str();
    }
}
