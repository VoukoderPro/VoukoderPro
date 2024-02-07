#include "sceneeditormuxernodemodel.h"

#include "ui/muxerpropertiesdialog.h"

/**
 * @brief SceneEditorMuxerNodeModel::SceneEditorMuxerNodeModel
 * @param nodeInfo
 * @param pluginMgr
 */
SceneEditorMuxerNodeModel::SceneEditorMuxerNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins):
    SceneEditorNodeModel(nodeInfo, plugins)
{
    // Initialize input codec ids
    for (unsigned int i = 0; i < nPorts(QtNodes::PortType::In); i++)
        inputInfos.push_back(std::make_pair(-1, ""));
}

/**
 * @brief SceneEditorMuxerNodeModel::nPorts
 * @param portType
 * @return
 */
unsigned int SceneEditorMuxerNodeModel::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
      case QtNodes::PortType::In:
        result = 8;
        break;

      case QtNodes::PortType::Out:
        result = 1;

      default:
        break;
    }

    return result;
}

/**
 * @brief SceneEditorMuxerNodeModel::dataType
 * @param portType
 * @param portIndex
 * @return
 */
QtNodes::NodeDataType SceneEditorMuxerNodeModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In)
    {
        return QtNodes::NodeDataType{ TYPE_STREAM, "stream" };
    }
    else
    {
        return QtNodes::NodeDataType{ TYPE_MUX, "mux" };
    }
}

/**
 * @brief SceneEditorMuxerNodeModel::setInData
 * @param previousNodeData
 * @param port
 */
void SceneEditorMuxerNodeModel::setInData(std::shared_ptr<QtNodes::NodeData> previousNodeData, const QtNodes::PortIndex port)
{
    // Update the input data
    if (previousNodeData)
    {
        std::shared_ptr<SceneEditorNodeData> encoderNodeData = std::static_pointer_cast<SceneEditorNodeData>(previousNodeData);

        inputInfos[port] = std::make_pair(
                    encoderNodeData->store[VPRO_DATA_CODEC_ID].toInt(),
                    encoderNodeData->store[VPRO_DATA_ENCODER_NAME].toString().toStdString());
    }
    else
        inputInfos[port] = std::make_pair(-1, "");

    validate();
}

/**
 * @brief SceneEditorMuxerNodeModel::hasProperties
 * @return
 */
bool SceneEditorMuxerNodeModel::hasProperties()
{
    return true;
}

/**
 * @brief SceneEditorMuxerNodeModel::showPropertyDialog
 */
void SceneEditorMuxerNodeModel::showPropertyDialog()
{
    // Filter muxer plugins
    std::vector<VoukoderPro::AssetInfo> muxerPlugins;
    filterPluginsByType(muxerPlugins, VoukoderPro::NodeInfoType::muxer);

    // Open the properties dialog
    MuxerPropertiesDialog dialog(_nodeInfo, muxerPlugins);
    if (dialog.exec() == QDialog::Accepted)
    {
        dialog.getValues(_nodeInfo->data);

        const std::string id = _nodeInfo->data.contains("id") ? _nodeInfo->data["id"].get<std::string>() : "";

        // Find the right plugin
        VoukoderPro::AssetInfo pluginInfo;
        if (findPluginById(id, _nodeInfo->type, pluginInfo))
            this->pluginInfo = pluginInfo;

        Q_EMIT embeddedWidgetSizeUpdated();
        Q_EMIT dataUpdated(0);
        Q_EMIT changed();

        validate();
    }
}

/**
 * @brief SceneEditorMuxerNodeModel::validate
 */
void SceneEditorMuxerNodeModel::validate()
{
    const std::string muxerId = _nodeInfo->data.contains("id") ? _nodeInfo->data["id"].get<std::string>() : "";

    // Find out if the input is valid
    VoukoderPro::AssetInfo pluginInfo;
    if (findPluginById(muxerId, _nodeInfo->type, pluginInfo))
    {
        for (const std::pair<int, std::string>& pair: inputInfos)
        {
            // Ignore all non-connected inputs
            if (pair.first == -1)
                continue;

            // Check connected inputs
            if (pluginInfo.allowedInputGroups.size() > 0 &&
                    std::find(pluginInfo.allowedInputGroups.begin(), pluginInfo.allowedInputGroups.end(), pair.first) == pluginInfo.allowedInputGroups.end())
            {
                // Codec not supported
                modelValidationState = QtNodes::NodeValidationState::Error;
                modelValidationMessage = tr("Unsupported: ") + (pair.first == -1 ? tr("<empty>") : QString::fromStdString(pair.second));

                return;
            }
        }

        // Codec supported
        modelValidationState = QtNodes::NodeValidationState::Valid;

        return;
    }

    // MuxerId is not known
    modelValidationState = QtNodes::NodeValidationState::Error;
    modelValidationMessage = tr("Muxer not known");
}
