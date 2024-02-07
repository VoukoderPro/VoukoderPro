#pragma once

#include "sceneeditornodemodel.h"

/**
 * @brief The SceneEditorInputNodeModel class
 */
class SceneEditorInputNodeModel : public SceneEditorNodeModel
{
    Q_OBJECT

public:
    SceneEditorInputNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins);
    virtual ~SceneEditorInputNodeModel() {};

    unsigned int nPorts(QtNodes::PortType portType) const override;
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    QWidget *embeddedWidget() override;
    virtual bool hasProperties() override;
    virtual void showPropertyDialog() override;
};
