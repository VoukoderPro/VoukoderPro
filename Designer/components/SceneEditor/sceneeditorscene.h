#pragma once

#include <nodes/FlowScene>
#include <QMenu>

#include "../VoukoderPro/voukoderpro_api.h"

/**
 * @brief The SceneEditorScene class
 */
class SceneEditorScene : public QtNodes::FlowScene
{
    Q_OBJECT

public:
    explicit SceneEditorScene(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, std::vector<VoukoderPro::AssetInfo> plugins, QObject *parent = nullptr);
    ~SceneEditorScene();

    void setSceneInfo(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo);
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo();
    QtNodes::Node* addNode(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo);
    bool isModified();
    void markNotModified(bool modified = true);

Q_SIGNALS:
    void changed();

private:
    std::size_t hashCode();
    void onNodePlaced(QtNodes::Node& node);
    void onNodeDeleted(QtNodes::Node& node);
    void onNodeMoved(QtNodes::Node& node, const QPointF& newLocation);
    void onConnectionCreated(QtNodes::Connection const &connection);
    void onConnectionDeleted(QtNodes::Connection const &connection);

private:
    std::vector<VoukoderPro::AssetInfo> plugins;
    std::map<QUuid, std::string> connectionMap;
    std::shared_ptr<VoukoderPro::SceneInfo> model;
    std::size_t state;
};
