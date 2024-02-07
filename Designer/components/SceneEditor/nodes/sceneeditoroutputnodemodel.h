#pragma once

#include "sceneeditornodemodel.h"

/**
 * @brief The SceneEditorOutputNodeModel class
 */
class SceneEditorOutputNodeModel : public SceneEditorNodeModel
{
    Q_OBJECT

public:
    SceneEditorOutputNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins);
    virtual ~SceneEditorOutputNodeModel() {};

    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    bool hasProperties() override;
    void showPropertyDialog() override;
};
