#pragma once

#include "sceneeditornodemodel.h"

/**
 * @brief The SceneEditorPostProcNodeModel class
 */
class SceneEditorPostProcNodeModel : public SceneEditorNodeModel
{
    Q_OBJECT

public:
    SceneEditorPostProcNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins);
    virtual ~SceneEditorPostProcNodeModel() {};

    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    bool hasProperties() override;
    void showPropertyDialog() override;
};


