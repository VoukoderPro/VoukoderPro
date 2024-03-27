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

    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    bool hasProperties() override;
    void showPropertyDialog() override;
};
