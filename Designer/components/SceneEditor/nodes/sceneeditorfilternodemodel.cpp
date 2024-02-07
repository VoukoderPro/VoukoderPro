#include "sceneeditorfilternodemodel.h"

#include "ui/filterpropertiesdialog.h"

/**
 * @brief SceneEditorFilterNodeModel::SceneEditorFilterNodeModel
 * @param nodeInfo
 * @param pluginMgr
 */
SceneEditorFilterNodeModel::SceneEditorFilterNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins):
    SceneEditorNodeModel(nodeInfo, plugins)
{}

/**
 * @brief SceneEditorFilterNodeModel::hasProperties
 * @return
 */
bool SceneEditorFilterNodeModel::hasProperties()
{
    return true;
}

/**
 * @brief SceneEditorFilterNodeModel::showPropertyDialog
 */
void SceneEditorFilterNodeModel::showPropertyDialog()
{
    // Filter filter plugins
    std::vector<VoukoderPro::AssetInfo> filterPlugins;
    filterPluginsByType(filterPlugins, VoukoderPro::NodeInfoType::filter);

    // Open the properties dialog
    FilterPropertiesDialog dialog(_nodeInfo, filterPlugins);
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
