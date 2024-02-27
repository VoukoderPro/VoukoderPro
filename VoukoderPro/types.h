#pragma once

#include <string>
#include <map>
#include <variant>
#include <functional>
#include <unordered_map>
#include <typeindex>
#include <algorithm> // std::transform
#include <iterator>  // std::back_inserter

#include <boost/describe/enum.hpp>
#include <boost/container_hash/hash.hpp>

#include "nlohmann/json.hpp"
#include "Errors.h"

namespace VoukoderPro
{
    typedef std::vector<std::vector<std::string>> pinId;
    typedef std::variant<int, double, std::string> configDataType;
    typedef std::map<std::string, configDataType> configType;

    BOOST_DEFINE_ENUM_CLASS(NodeInfoType, input, filter, encoder, muxer, output, postproc );
    BOOST_DEFINE_ENUM_CLASS(MediaType, video, audio, mux, out );

    typedef unsigned flags;

    enum class ItemParamType
    {
        Unknown = 0,
        Flag,
        String,
        Integer,
        Double,
        Boolean
    };

    template<typename T>
    class ItemParam;

    class ItemParams;

    struct ItemParamAction
    {
        std::function<void(std::string id)> setEdit;
        std::function<void(std::string id, bool visibility)> setVisible;
        std::function<void(std::string id, std::string value)> setProperty;
    };

    class ItemParamOptionBase
    {
    public:
        ItemParamOptionBase(std::string text, std::function<void(ItemParamAction&)> onSelect = [](const ItemParamAction&) {}):
            _text(text), _onSelect(onSelect)
        {}

        std::string text()
        {
            return _text;
        }

        void callSelected(ItemParamAction& action)
        {
            _onSelect(action);
        }

    private:
        std::string _text;
        std::function<void(ItemParamAction&)> _onSelect;
    };

    template<typename T>
    class ItemParamOption : public ItemParamOptionBase
    {
    public:
        ItemParamOption(std::string text, T value, std::function<void(ItemParamAction&)> onSelect = [](const ItemParamAction&) {}):
            ItemParamOptionBase(text, onSelect), _value(value)
        {}

        T value()
        {
            return _value;
        }

    private:
        T _value;
    };

    enum ItemParamGroupType
    {
        Standard,
        Forced
    };

    class ItemParamBase
    {
    public:
        std::string id()
        {
            return _id;
        }

        std::string name()
        {
            return _name;
        }

        std::string label()
        {
            return _label;
        }

        ItemParamType type()
        {
            return _type;
        }

        int indent()
        {
            return _indent;
        }

        std::vector<ItemParamOptionBase*> options()
        {
            return _options;
        }

    protected:
        ItemParamBase(std::string id, std::string name, std::string label, std::type_index typeindex, int indent) :
            _id(id), _name(name), _label(label), _indent(indent)
        {
            if (typeindex == typeid(VoukoderPro::flags))
                _type = ItemParamType::Flag;
            else if (typeindex == typeid(std::string))
                _type = ItemParamType::String;
            else if (typeindex == typeid(int))
                _type = ItemParamType::Integer;
            else if (typeindex == typeid(double))
                _type = ItemParamType::Double;
            else if (typeindex == typeid(bool))
                _type = ItemParamType::Boolean;
            else
                _type = ItemParamType::Unknown;
        }

    protected:
        std::string _id;
        std::string _name;
        std::string _label;
        int _indent;
        std::vector<ItemParamOptionBase*> _options;

    private:
        ItemParamType _type;
    };

    template<typename T>
    class ItemParam : public ItemParamBase
    {
    public:
        ItemParam(std::string id, std::string name, std::string label, int indent) :
            ItemParamBase(id, name, label, typeid(T), indent)
        {}

        ItemParam& option(std::string text, T value, std::function<void(ItemParamAction&)> onSelect = [](ItemParamAction&) {})
        {
            ItemParamOption<T>* option = new ItemParamOption<T>(text, value, onSelect);
            _options.push_back(option);

            if (value < _min)
                _min = value;

            if (value > _max)
                _max = value;

            return *this;
        }

        ItemParam& flag(std::string text)
        {
            return option(text, 1 << _options.size());
        }

        ItemParam& description(std::string description)
        {
            _description = description;
            return *this;
        }

        std::string description()
        {
            return _description;
        }

        ItemParam& allowCustomValues(bool allowCustomValues = true)
        {
            _allowCustomValues = allowCustomValues;
            return *this;
        }

        ItemParam& ignore(const bool ignore)
        {
            _ignore = ignore;
            return *this;
        }

        void visible(const bool visible)
        {
            this->onVisible(_id, visible);
        }

        ItemParam& minValue(T value)
        {
            _min = value;
            return *this;
        }

        ItemParam& maxValue(T value)
        {
            _max = value;
            return *this;
        }

        ItemParam& multiplierValue(int value)
        {
            _multiplier = value;
            return *this;
        }

        ItemParam& defaultValue(T value)
        {
            _default = value;
            return *this;
        }

        T def()
        {
            return _default;
        }

        T minimum()
        {
            return _min;
        }

        T maximum()
        {
            return _max;
        }

        int multiplier()
        {
            return _multiplier;
        }

        bool ignore()
        {
            return _ignore;
        }

        T valueFrom(nlohmann::ordered_json& params)
        {
            return params.contains(_id) ? params.at(_id).get<T>() : _default;
        }

    private:
        std::string _description;
        std::string optionDefault;
        bool _allowCustomValues = false;
        std::function<void(std::string, bool)> onVisible;

        T _default;
        T _min;
        T _max;
        int _multiplier = 1;
        bool _ignore = false;
    };

    struct ItemParamGroup
    {
        ItemParamGroup(const std::string id, const std::string name, const ItemParamGroupType type):
            _id(id), _name(name), _type(type)
        {}

        template<typename T>
        ItemParam<T>& param(const std::string id, const std::string name, int indent = 0)
        {
            ItemParam<T>* param = new ItemParam<T>(_id + "." + id, id, name, indent);
            params.push_back(param);
            return *param;
        }

        std::string id()
        {
            return _id;
        }

        std::string name()
        {
            return _name;
        }

        ItemParamGroupType type()
        {
            return _type;
        }

        std::vector<ItemParamBase*> params;

    private:
        std::string _id;
        std::string _name;
        ItemParamGroupType _type;
    };

    struct ItemFormat
    {
        ItemFormat(const std::string id, const std::string name, std::function<void(ItemParamAction&)> onSelect = [](ItemParamAction&) {}):
            _id(id), _name(name)
        {}

        std::string id()
        {
            return _id;
        }

        std::string name()
        {
            return _name;
        }

    private:
        std::string _id;
        std::string _name;
    };

    struct AssetPreset
    {
        AssetPreset(const std::string name, const std::map<std::string, std::string> params = {}) :
            _name(name), _params(params)
        {}

        std::string name()
        {
            return _name;
        }

    private:
        std::string _name;
        std::map<std::string, std::string> _params;
    };

    struct AssetInfo
    {
        std::string id;
        std::string name;
        std::string description;
        std::string helpUrl;
        int codec = 0;
        std::vector<ItemFormat*> formats;
        std::pair<std::string, std::string> category;
        NodeInfoType type;
        MediaType mediaType;
        std::vector<int> allowedInputGroups;
        std::unordered_map<std::string, ItemParamGroup*> groups;
        std::unordered_map<std::string, AssetPreset*> presets;

        AssetPreset& preset(const std::string name, const std::map<std::string, std::string> params)
        {
            AssetPreset* preset = new AssetPreset(name, params);
            presets.insert(std::make_pair(name, preset));

            return *preset;
        }

        ItemParamGroup& group(const std::string id, const std::string name, const ItemParamGroupType type)
        {
            ItemParamGroup* group = new ItemParamGroup(id, name, type);
            groups.insert(std::make_pair(id, group));

            return *group;
        }

        AssetInfo& format(const std::string text, const std::string value, std::function<void(ItemParamAction&)> onSelect = [](ItemParamAction&) {})
        {
            ItemFormat* format = new ItemFormat(value, text, onSelect);
            formats.push_back(format);

            return *this;
        }
    };

    struct NodeInfo
    {
        std::string id;
        NodeInfoType type;
        MediaType mediaType;
        double posX, posY;
        nlohmann::ordered_json data;
        std::vector<std::vector<std::string>> inputs;
        std::vector<std::vector<std::string>> outputs;

        const std::size_t hashCode()
        {
            std::size_t seed = 0;
            boost::hash_combine(seed, std::hash<std::string>{}(id));
            boost::hash_combine(seed, std::hash<NodeInfoType>{}(type));
            boost::hash_combine(seed, std::hash<MediaType>{}(mediaType));
            boost::hash_combine(seed, std::hash<double>{}(posX));
            boost::hash_combine(seed, std::hash<double>{}(posY));
            boost::hash_combine(seed, std::hash<nlohmann::ordered_json>{}(data));

            for (int i = 0; i < inputs.size(); i++)
                for (int j = 0; j < inputs.at(i).size(); j++)
                    boost::hash_combine(seed, std::hash<std::string>{}(std::to_string(i) + "-" + std::to_string(j) + "-" + inputs.at(i).at(j)));

            for (int i = 0; i < outputs.size(); i++)
                for (int j = 0; j < outputs.at(i).size(); j++)
                    boost::hash_combine(seed, std::hash<std::string>{}(std::to_string(i) + "-" + std::to_string(j) + "-" + outputs.at(i).at(j)));

            return seed;
        }

        bool operator ==(const NodeInfo& other)
        {
            return this == &other;
        }
    };

    struct SceneInfo
    {
        std::string name;
        std::vector<std::shared_ptr<VoukoderPro::NodeInfo>> nodes;

        const std::size_t hashCode()
        {
            std::size_t seed = 0;
            boost::hash_combine(seed, std::hash<std::string>{}(name));

            for (auto& node : nodes)
                boost::hash_combine(seed, node->hashCode());

            return seed;
        }
    };
}
