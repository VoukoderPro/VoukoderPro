#pragma once

#include <nodes/NodeDataModel>
#include <nodes/NodeData>

#include "../VoukoderPro/voukoderpro_api.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

#include "include/QtPropertyBrowser/qttreepropertybrowser.h"
#include "include/QtPropertyBrowser/qtvariantproperty.h"
#include "sceneeditornodedata.h"

#define TYPE_RAW_VIDEO "uncompressed_video"
#define TYPE_RAW_AUDIO "uncompressed_audio"
#define TYPE_STREAM "stream"
#define TYPE_VIDEO "compressed_video"
#define TYPE_AUDIO "compressed_audio"
#define TYPE_MUX "mux"
#define TYPE_OUT "out"

/**
 * @brief The SceneEditorNodeModel class
 */
class SceneEditorNodeModel : public QtNodes::NodeDataModel
{
    Q_OBJECT

public:
    SceneEditorNodeModel(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, std::vector<VoukoderPro::AssetInfo> plugins);
    virtual ~SceneEditorNodeModel();

    QString caption() const override;
    QString name() const override;

    virtual void setInData(std::shared_ptr<QtNodes::NodeData> previousNodeData, const QtNodes::PortIndex port) override;
    virtual std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;
    virtual void init();
    virtual std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo();
    virtual bool hasProperties();
    virtual QtNodes::NodeValidationState validationState() const override;
    virtual QString validationMessage() const override;
    virtual void showPropertyDialog() {};
    QWidget *embeddedWidget() override;

protected:
    std::shared_ptr<VoukoderPro::NodeInfo> _nodeInfo;
    QLabel* label = nullptr;
    std::vector<VoukoderPro::AssetInfo> plugins;
    std::shared_ptr<QtNodes::NodeData> nodeData;
    QtNodes::NodeValidationState modelValidationState = QtNodes::NodeValidationState::Valid;
    QString modelValidationMessage = "";
    VoukoderPro::AssetInfo pluginInfo;

    void filterPluginsByType(std::vector<VoukoderPro::AssetInfo>& plugins, VoukoderPro::NodeInfoType type)
    {
        for (auto& plugin : this->plugins)
        {
            if (plugin.type == type)
                plugins.push_back(plugin);
        }
    }

    bool findPluginById(const std::string id, VoukoderPro::NodeInfoType type, VoukoderPro::AssetInfo& pluginInfo)
    {
        for (auto& plugin : this->plugins)
        {
            if (plugin.id == id && plugin.type == type)
            {
                pluginInfo = plugin;
                return true;
            }
        }

        return false;
    }

Q_SIGNALS:
    void changed();
};
