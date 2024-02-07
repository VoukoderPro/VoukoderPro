#include "sceneeditorencodernodemodel.h"

#include "ui/encoderpropertiesdialog.h"

/**
 * @brief SceneEditorEncoderNodeModel::SceneEditorEncoderNodeModel
 * @param nodeInfo
 * @param pluginMgr
 */
SceneEditorEncoderNodeModel::SceneEditorEncoderNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins):
    SceneEditorNodeModel(nodeInfo, plugins)
{
    validate();
}

/**
 * @brief SceneEditorEncoderNodeModel::nPorts
 * @param portType
 * @return
 */
unsigned int SceneEditorEncoderNodeModel::nPorts(QtNodes::PortType portType) const
{
    return 1;
}

/**
 * @brief SceneEditorEncoderNodeModel::dataType
 * @param portType
 * @param portIndex
 * @return
 */
QtNodes::NodeDataType SceneEditorEncoderNodeModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In)
    {
        if (_nodeInfo->mediaType == VoukoderPro::MediaType::video)
            return QtNodes::NodeDataType{ TYPE_RAW_VIDEO, "video" };
        else
            return QtNodes::NodeDataType{ TYPE_RAW_AUDIO, "audio" };
    }
    else
    {
        return QtNodes::NodeDataType{ TYPE_STREAM, "stream" };
    }
}

/**
 * @brief SceneEditorEncoderNodeModel::setInData
 * @param previousNodeData
 * @param port
 */
void SceneEditorEncoderNodeModel::setInData(std::shared_ptr<QtNodes::NodeData> previousNodeData, const QtNodes::PortIndex port)
{}

/**
 * @brief SceneEditorEncoderNodeModel::hasProperties
 * @return
 */
bool SceneEditorEncoderNodeModel::hasProperties()
{
    return true;
}

/**
 * @brief SceneEditorEncoderNodeModel::showPropertyDialog
 */
void SceneEditorEncoderNodeModel::showPropertyDialog()
{
    // Filter encoder plugins
    std::vector<VoukoderPro::AssetInfo> encoderPlugins;
    filterPluginsByType(encoderPlugins, VoukoderPro::NodeInfoType::encoder);

    // Open the properties dialog
    EncoderPropertiesDialog dialog(_nodeInfo, encoderPlugins);
    if (dialog.exec() == QDialog::Accepted)
    {
        dialog.getValues(_nodeInfo->data);

        const std::string id = _nodeInfo->data.contains("id") ? _nodeInfo->data["id"].get<std::string>() : "";

        // Find the right plugin
        VoukoderPro::AssetInfo pluginInfo;
        if (findPluginById(id, _nodeInfo->type, pluginInfo))
        {
            this->pluginInfo = pluginInfo;

            // Create a data object that gets pushed to the next node
            std::shared_ptr<SceneEditorNodeData> data = std::static_pointer_cast<SceneEditorNodeData>(nodeData);
            data->store[VPRO_DATA_CODEC_ID] = QVariant::fromValue(pluginInfo.codec);
            data->store[VPRO_DATA_ENCODER_NAME] = QVariant::fromValue(QString::fromStdString(pluginInfo.name));
        }

        Q_EMIT embeddedWidgetSizeUpdated();
        Q_EMIT dataUpdated(0);
        Q_EMIT changed();

        validate();
    }
}

/**
 * @brief SceneEditorEncoderNodeModel::validate
 */
void SceneEditorEncoderNodeModel::validate()
{
    const QString encoderId = _nodeInfo->data.contains("id") ? QString::fromStdString(_nodeInfo->data["id"].get<std::string>()) : "";

    // Try to find the encoder plugin
    VoukoderPro::AssetInfo pluginInfo;
    if (findPluginById(encoderId.toStdString(), _nodeInfo->type, pluginInfo))
    {
        std::shared_ptr<SceneEditorNodeData> data = std::static_pointer_cast<SceneEditorNodeData>(nodeData);

        // Add the codec id to the store
        data->store[VPRO_DATA_CODEC_ID] = QVariant::fromValue(pluginInfo.codec);
        data->store[VPRO_DATA_ENCODER_NAME] = QVariant::fromValue(QString::fromStdString(pluginInfo.name));

        modelValidationState = QtNodes::NodeValidationState::Valid;

        return;
    }

    // Encoder is not known
    modelValidationState = QtNodes::NodeValidationState::Error;
    modelValidationMessage = tr("Encoder not known") + ": " + (encoderId.isEmpty() ? tr("<empty>") : encoderId);
}
