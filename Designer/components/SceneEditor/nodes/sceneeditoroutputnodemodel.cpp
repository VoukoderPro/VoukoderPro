#include "sceneeditoroutputnodemodel.h"

#include "ui/outputpropertiesdialog.h"

/**
 * @brief SceneEditorOutputNodeModel::SceneEditorOutputNodeModel
 * @param nodeInfo
 * @param pluginMgr
 */
SceneEditorOutputNodeModel::SceneEditorOutputNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins):
    SceneEditorNodeModel(nodeInfo, plugins)
{}

/**
 * @brief SceneEditorOutputNodeModel::nPorts
 * @param portType
 * @return
 */
unsigned int SceneEditorOutputNodeModel::nPorts(QtNodes::PortType portType) const
{
    return 1;
}

/**
 * @brief SceneEditorOutputNodeModel::dataType
 * @param portType
 * @param portIndex
 * @return
 */
QtNodes::NodeDataType SceneEditorOutputNodeModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In)
    {
        return QtNodes::NodeDataType{ TYPE_MUX, "mux" };
    }
    else
    {
        return QtNodes::NodeDataType{ TYPE_OUT, "out" };
    }
}

/**
 * @brief SceneEditorOutputNodeModel::hasProperties
 * @return
 */
bool SceneEditorOutputNodeModel::hasProperties()
{
    return true;
}

/**
 * @brief SceneEditorOutputNodeModel::showPropertyDialog
 */
void SceneEditorOutputNodeModel::showPropertyDialog()
{
    // Filter output plugins
    std::vector<VoukoderPro::AssetInfo> outputPlugins;
    filterPluginsByType(outputPlugins, VoukoderPro::NodeInfoType::output);

    // Open the properties dialog
    OutputPropertiesDialog dialog(_nodeInfo, outputPlugins);
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
