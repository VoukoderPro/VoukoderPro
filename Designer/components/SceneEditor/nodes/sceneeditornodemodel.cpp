#include "sceneeditornodemodel.h"

#include <boost/describe/enum_to_string.hpp>
#include <QLabel>

/**
 * @brief SceneEditorNodeModel::SceneEditorNodeModel
 * @param nodeInfo
 * @param pluginMgr
 */
SceneEditorNodeModel::SceneEditorNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins):
    _nodeInfo{nodeInfo}, plugins(plugins), nodeData(std::make_shared<SceneEditorNodeData>())
{
    label = new QLabel;
    label->setMargin(5);
    label->setStyleSheet("QLabel { color: rgba(255, 255, 127, 255); }");

    pluginInfo.name = tr("Unknown").toStdString();
}

/**
 * @brief SceneEditorNodeModel::~SceneEditorNodeModel
 */
SceneEditorNodeModel::~SceneEditorNodeModel()
{}

/**
 * @brief SceneEditorNodeModel::init
 */
void SceneEditorNodeModel::init()
{
    if (label)
    {
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
        darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::Dark, QColor(35, 35, 35));
        darkPalette.setColor(QPalette::Shadow, QColor(20, 20, 20));
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
        darkPalette.setColor(QPalette::HighlightedText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));

        label->setPalette(darkPalette);
    }

    if (_nodeInfo->data.contains("id"))
    {
        const std::string id = _nodeInfo->data["id"].get<std::string>();

        VoukoderPro::AssetInfo pluginInfo;
        if (findPluginById(id, _nodeInfo->type, pluginInfo))
            this->pluginInfo = pluginInfo;
    }
}

/**
 * @brief SceneEditorNodeModel::caption
 * @return
 */
QString SceneEditorNodeModel::caption() const
{
    QString caption, nodeType;

    if (_nodeInfo->type == VoukoderPro::NodeInfoType::input || _nodeInfo->type == VoukoderPro::NodeInfoType::filter || _nodeInfo->type == VoukoderPro::NodeInfoType::encoder)
    {
        caption = QString::fromStdString(boost::describe::enum_to_string<VoukoderPro::MediaType>(_nodeInfo->mediaType, ""));
        if (!caption.isEmpty())
            caption.replace(0, 1, caption[0].toUpper());
    }

    nodeType = QString::fromStdString(boost::describe::enum_to_string<VoukoderPro::NodeInfoType>(_nodeInfo->type, ""));
    if (!nodeType.isEmpty())
        nodeType.replace(0, 1, nodeType[0].toUpper());

    if (!caption.isEmpty() || !nodeType.isEmpty())
        caption += " ";

    return caption + nodeType;
}

/**
 * @brief SceneEditorNodeModel::name
 * @return
 */
QString SceneEditorNodeModel::name() const
{
    return QString::fromStdString(boost::describe::enum_to_string<VoukoderPro::NodeInfoType>(_nodeInfo->type, ""));
}

/**
 * @brief SceneEditorNodeModel::setInData
 * @param previousNodeData
 * @param port
 */
void SceneEditorNodeModel::setInData(std::shared_ptr<QtNodes::NodeData> previousNodeData, const QtNodes::PortIndex port)
{
    this->nodeData = previousNodeData;
}

/**
 * @brief SceneEditorNodeModel::outData
 * @param port
 * @return
 */
std::shared_ptr<QtNodes::NodeData> SceneEditorNodeModel::outData(const QtNodes::PortIndex port)
{
    return nodeData;
}

/**
 * @brief SceneEditorNodeModel::nodeInfo
 * @return
 */
std::shared_ptr<VoukoderPro::NodeInfo> SceneEditorNodeModel::nodeInfo()
{
    return _nodeInfo;
}

/**
 * @brief SceneEditorNodeModel::embeddedWidget
 * @return
 */
QWidget* SceneEditorNodeModel::embeddedWidget()
{
    label->setText(QString::fromStdString(pluginInfo.name));

    return label;
}

/**
 * @brief SceneEditorNodeModel::validationState
 * @return
 */
QtNodes::NodeValidationState SceneEditorNodeModel::validationState() const
{
    return modelValidationState;
}

/**
 * @brief SceneEditorNodeModel::validationMessage
 * @return
 */
QString SceneEditorNodeModel::validationMessage() const
{
    return modelValidationMessage;
}

/**
 * @brief SceneEditorNodeModel::hasProperties
 * @return
 */
bool SceneEditorNodeModel::hasProperties()
{
    return false;
}
