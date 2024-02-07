#include "sceneeditorinputnodemodel.h"

#include "ui/inputpropertiesdialog.h"

/**
 * @brief SceneEditorInputNodeModel::SceneEditorInputNodeModel
 * @param nodeInfo
 * @param pluginMgr
 */
SceneEditorInputNodeModel::SceneEditorInputNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins):
    SceneEditorNodeModel(nodeInfo, plugins)
{}

/**
 * @brief SceneEditorInputNodeModel::nPorts
 * @param portType
 * @return
 */
unsigned int SceneEditorInputNodeModel::nPorts(QtNodes::PortType portType) const
{
    return portType == QtNodes::PortType::In ? 0 : 1;
}

/**
 * @brief SceneEditorInputNodeModel::dataType
 * @param portType
 * @param portIndex
 * @return
 */
QtNodes::NodeDataType SceneEditorInputNodeModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (_nodeInfo->mediaType == VoukoderPro::MediaType::video)
        return QtNodes::NodeDataType{ TYPE_RAW_VIDEO, "video" };
    else
        return QtNodes::NodeDataType{ TYPE_RAW_AUDIO, "audio" };
}

/**
 * @brief SceneEditorInputNodeModel::embeddedWidget
 * @return
 */
QWidget* SceneEditorInputNodeModel::embeddedWidget()
{
    if (_nodeInfo->data.contains("tracks"))
    {
        const auto& tracks = _nodeInfo->data["tracks"];

        // No track numbers are specified -> the first track matches all nle tracks
        if (tracks.size() == 0)
            label->setText("All tracks");
        else
        {
            // A track number is specified ...
            QStringList trks;
            for (const auto& track : tracks)
                trks.append(QString::number(track.get<int>()));

            label->setText("Track " + trks.join(","));
        }
    }
    else
        label->setText("All tracks");

    return label;
}

/**
 * @brief SceneEditorInputNodeModel::hasProperties
 * @return
 */
bool SceneEditorInputNodeModel::hasProperties()
{
    return true;
}

/**
 * @brief SceneEditorInputNodeModel::showPropertyDialog
 */
void SceneEditorInputNodeModel::showPropertyDialog()
{
    // Open the properties dialog
    InputPropertiesDialog dialog(_nodeInfo);
    if (dialog.exec() == QDialog::Accepted)
    {
        dialog.getValues(_nodeInfo->data);

        Q_EMIT embeddedWidgetSizeUpdated();
        Q_EMIT dataUpdated(0);
        Q_EMIT changed();
    }
}
