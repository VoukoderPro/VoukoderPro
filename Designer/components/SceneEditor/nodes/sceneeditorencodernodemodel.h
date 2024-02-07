#pragma once

#include "sceneeditornodemodel.h"

/**
 * @brief The SceneEditorEncoderNodeModel class
 */
class SceneEditorEncoderNodeModel : public SceneEditorNodeModel
{
    Q_OBJECT

public:
    SceneEditorEncoderNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins);
    virtual ~SceneEditorEncoderNodeModel() {};

    // QtNodes::NodeDataModel
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    void setInData(std::shared_ptr<QtNodes::NodeData> previousNodeData, const QtNodes::PortIndex port) override;

    // SceneEditorNodeModel
    bool hasProperties() override;
    void showPropertyDialog() override;

private:
    void validate();
};
