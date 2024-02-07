#include "sceneeditorpostprocnodemodel.h"
#include "ui/postprocpropertiesdialog.h"

/**
 * @brief SceneEditorPostProcNodeModel::SceneEditorPostProcNodeModel
 * @param nodeInfo
 * @param pluginMgr
 */
SceneEditorPostProcNodeModel::SceneEditorPostProcNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins):
    SceneEditorNodeModel(nodeInfo, plugins)
{}

/**
 * @brief SceneEditorPostProcNodeModel::nPorts
 * @param portType
 * @return
 */
unsigned int SceneEditorPostProcNodeModel::nPorts(QtNodes::PortType portType) const
{
    return portType == QtNodes::PortType::In ? 1 : 0;
}

/**
 * @brief SceneEditorPostProcNodeModel::dataType
 * @param portType
 * @param portIndex
 * @return
 */
QtNodes::NodeDataType SceneEditorPostProcNodeModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return QtNodes::NodeDataType{ TYPE_OUT, "out" };
}

/**
 * @brief SceneEditorPostProcNodeModel::hasProperties
 * @return
 */
bool SceneEditorPostProcNodeModel::hasProperties()
{
    return true;
}

void SceneEditorPostProcNodeModel::showPropertyDialog()
{
    // Filter postproc plugins
    std::vector<VoukoderPro::AssetInfo> outputPlugins;
    filterPluginsByType(outputPlugins, VoukoderPro::NodeInfoType::postproc);

    // Open the properties dialog
    PostProcPropertiesDialog dialog(_nodeInfo, plugins);
    if (dialog.exec() == QDialog::Accepted)
    {
        dialog.getValues(_nodeInfo->data);

        const std::string id = _nodeInfo->data.contains("id") ? _nodeInfo->data["id"].get<std::string>() : "";

        // Find the right plugin
        VoukoderPro::AssetInfo pluginInfo;
        if (findPluginById(id, _nodeInfo->type, pluginInfo))
            this->pluginInfo = pluginInfo;

        Q_EMIT embeddedWidgetSizeUpdated();
        Q_EMIT changed();
    }
}

