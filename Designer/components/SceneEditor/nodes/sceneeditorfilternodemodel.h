#pragma once

#include "sceneeditornodemodel.h"

/**
 * @brief The SceneEditorFilterNodeModel class
 */
class SceneEditorFilterNodeModel : public SceneEditorNodeModel
{
    Q_OBJECT

public:
    SceneEditorFilterNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins);
    virtual ~SceneEditorFilterNodeModel() {};

    unsigned int nPorts(QtNodes::PortType portType) const override
    {
        return 1;
    }

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override
    {
        if (_nodeInfo->mediaType == VoukoderPro::MediaType::video)
            return QtNodes::NodeDataType{ TYPE_RAW_VIDEO, "video" };
        else
            return QtNodes::NodeDataType{ TYPE_RAW_AUDIO, "audio" };
    }

    bool hasProperties() override;
    void showPropertyDialog() override;
};
