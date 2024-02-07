#pragma once

#include "sceneeditornodemodel.h"

#include <nodes/internal/NodeDataModel.hpp>

/**
 * @brief The SceneEditorMuxerNodeModel class
 */
class SceneEditorMuxerNodeModel : public SceneEditorNodeModel
{
    Q_OBJECT

public:
    SceneEditorMuxerNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins);
    virtual ~SceneEditorMuxerNodeModel() {};

    // QtNodes::NodeDataModel
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    void setInData(std::shared_ptr<QtNodes::NodeData> previousNodeData, const QtNodes::PortIndex port) override;

    // SceneEditorNodeModel
    bool hasProperties() override;
    void showPropertyDialog() override;

private:
    void validate();

private:
    std::vector<std::pair<int, std::string>> inputInfos;
};

