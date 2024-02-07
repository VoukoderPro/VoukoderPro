#include "sceneeditorscene.h"

#include <nodes/internal/NodeGraphicsObject.hpp>
#include <nodes/internal/PortType.hpp>
#include <nodes/Node>
#include "nodes/sceneeditorinputnodemodel.h"
#include "nodes/sceneeditorfilternodemodel.h"
#include "nodes/sceneeditorencodernodemodel.h"
#include "nodes/sceneeditormuxernodemodel.h"
#include "nodes/sceneeditoroutputnodemodel.h"
#include "nodes/sceneeditorpostprocnodemodel.h"

/**
 * @brief SceneEditorScene::SceneEditorScene
 * @param sceneInfo
 * @param pluginMgr
 * @param parent
 */
SceneEditorScene::SceneEditorScene(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, std::vector<VoukoderPro::AssetInfo> plugins, QObject *parent)
    : QtNodes::FlowScene{parent}, plugins(plugins), model({}), state((size_t) - 1)
{
    setSceneRect({});

    setSceneInfo(sceneInfo);

    connect(this, &SceneEditorScene::nodeDoubleClicked, this, [](QtNodes::Node& node)
    {
        SceneEditorNodeModel* model = static_cast<SceneEditorNodeModel*>(node.nodeDataModel());
        if (model && model->hasProperties())
            model->showPropertyDialog();
    });

    // What events could a scene cause to be modified?
    connect(this, &QtNodes::FlowScene::nodePlaced, this, &SceneEditorScene::onNodePlaced);
    connect(this, &QtNodes::FlowScene::nodeDeleted, this, &SceneEditorScene::onNodeDeleted);
    connect(this, &QtNodes::FlowScene::nodeMoved, this, &SceneEditorScene::onNodeMoved);
    connect(this, &QtNodes::FlowScene::connectionCreated, this, &SceneEditorScene::onConnectionCreated);
    connect(this, &QtNodes::FlowScene::connectionDeleted, this, &SceneEditorScene::onConnectionDeleted);
}

/**
 * @brief SceneEditorScene::~SceneEditorScene
 */
SceneEditorScene::~SceneEditorScene()
{
    disconnect(this);
}

/**
 * @brief SceneEditorScene::setSceneInfo
 * @param sceneInfo
 */
void SceneEditorScene::setSceneInfo(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo)
{
    // Create a copy
    this->model = sceneInfo;

    std::map<std::string, std::tuple<QtNodes::Node*, QtNodes::PortIndex>> incompleteConnections;

    auto updateNodeIds = [](std::shared_ptr<QtNodes::Connection> connection, std::string oldId)
    {
        const std::string id = connection->id().toString(QUuid::WithoutBraces).toStdString();

        // Input
        {
            QtNodes::Node* node = connection->getNode(QtNodes::PortType::In);
            SceneEditorNodeModel* nodeModel = static_cast<SceneEditorNodeModel*>(node->nodeDataModel());
            QtNodes::PortIndex portIndex = connection->getPortIndex(QtNodes::PortType::In);
            auto nodeInfo = nodeModel->nodeInfo();

            std::vector<std::string>& ids = nodeInfo->inputs.at(portIndex);
            auto f = std::find(ids.begin(), ids.end(), oldId);
            if (f != ids.end())
                *f = id;
        }

        // Output
        {
            QtNodes::Node* node = connection->getNode(QtNodes::PortType::Out);
            SceneEditorNodeModel* nodeModel = static_cast<SceneEditorNodeModel*>(node->nodeDataModel());
            QtNodes::PortIndex portIndex = connection->getPortIndex(QtNodes::PortType::Out);
            auto nodeInfo = nodeModel->nodeInfo();

            std::vector<std::string>& ids = nodeInfo->outputs.at(portIndex);
            auto f = std::find(ids.begin(), ids.end(), oldId);
            if (f != ids.end())
                *f = id;
        }
    };

    // Connect the nodes with existing connections
    auto connectNodes = [&](QtNodes::Node& node, const std::vector<std::vector<std::string>> pins, const QtNodes::PortType type)
    {
        const QtNodes::PortIndex total = pins.size();
        for (QtNodes::PortIndex index = 0; index < total; index++)
        {
            const auto& ids = pins.at(index);
            for (std::string id : ids)
            {
                const auto it = incompleteConnections.find(id);
                if (it != incompleteConnections.end())
                {
                    const auto& [otherNode, otherIndex] = incompleteConnections.at(id);

                    // Create the connection
                    if (type == QtNodes::PortType::In)
                    {
                        const auto connection = createConnection(node, index, *otherNode, otherIndex);
                        updateNodeIds(connection, id);

                        connect(connection.get(), &QtNodes::Connection::updated, this, &SceneEditorScene::onConnectionCreated);

                        markNotModified(false);

                        //Q_EMIT changed();
                    }
                    else if (type == QtNodes::PortType::Out)
                    {
                        const auto connection = createConnection(*otherNode, otherIndex, node, index);
                        updateNodeIds(connection, id);

                        connect(connection.get(), &QtNodes::Connection::updated, this, &SceneEditorScene::onConnectionCreated);

                        markNotModified(false);

                        //Q_EMIT changed();
                    }
                    else
                        continue;

                    incompleteConnections.erase(it);
                }
                else
                {
                    const auto pair = std::make_pair(id, std::make_tuple(&node, index));
                    incompleteConnections.insert(pair);
                }
            }
        }
    };

    // Handle all nodes in the scene
    for (const auto& nodeInfo : model->nodes)
    {
        QtNodes::Node* node = addNode(nodeInfo);
        if (node)
        {
            // Handle node connections
            connectNodes(*node, nodeInfo->inputs, QtNodes::PortType::In);
            connectNodes(*node, nodeInfo->outputs, QtNodes::PortType::Out);
        }
    }
}

/**
 * @brief SceneEditorScene::sceneInfo
 * @return
 */
std::shared_ptr<VoukoderPro::SceneInfo> SceneEditorScene::sceneInfo()
{
    return model;
}

/**
 * @brief SceneEditorScene::hashCode
 * @return
 */
std::size_t SceneEditorScene::hashCode()
{
    return model->hashCode();
}

/**
 * @brief SceneEditorScene::isModified
 * @return
 */
bool SceneEditorScene::isModified()
{
    return state != hashCode();
}

/**
 * @brief SceneEditorScene::markNotModified
 * @param modified
 */
void SceneEditorScene::markNotModified(bool modified)
{
    state = modified ? hashCode() : (size_t) - 1;

    Q_EMIT changed();
}

// ### EVENTS ###

/**
 * @brief SceneEditorScene::onNodePlaced
 * @param newNode
 */
void SceneEditorScene::onNodePlaced(QtNodes::Node& newNode)
{
    SceneEditorNodeModel* nodeModel = static_cast<SceneEditorNodeModel*>(newNode.nodeDataModel());
    if (nodeModel)
    {
        auto nodeInfo = nodeModel->nodeInfo();
        model->nodes.push_back(nodeInfo);
    }

    Q_EMIT changed();
}

/**
 * @brief SceneEditorScene::onNodeDeleted
 * @param node
 */
void SceneEditorScene::onNodeDeleted(QtNodes::Node& node)
{
    SceneEditorNodeModel* nodeModel = static_cast<SceneEditorNodeModel*>(node.nodeDataModel());
    if (nodeModel)
    {
        auto nodeInfo = nodeModel->nodeInfo();
        model->nodes.erase(std::remove(model->nodes.begin(), model->nodes.end(), nodeInfo));
    }

    Q_EMIT changed();
}

/**
 * @brief SceneEditorScene::onNodeMoved
 * @param node
 * @param newLocation
 */
void SceneEditorScene::onNodeMoved(QtNodes::Node& node, const QPointF& newLocation)
{
    SceneEditorNodeModel* nodeModel = static_cast<SceneEditorNodeModel*>(node.nodeDataModel());

    // Update the node position
    auto nodeInfo = nodeModel->nodeInfo();
    nodeInfo->posX = newLocation.x();
    nodeInfo->posY = newLocation.y();

    Q_EMIT changed();
}

/**
 * @brief SceneEditorScene::onConnectionCreated
 * @param connection
 */
void SceneEditorScene::onConnectionCreated(QtNodes::Connection const &connection)
{
    // Connection id
    const std::string id = connection.id().toString(QUuid::WithoutBraces).toStdString();

    for (QtNodes::PortType portType : { QtNodes::PortType::In, QtNodes::PortType::Out })
    {
        // Get the info of each node
        const QtNodes::Node* node = connection.getNode(portType);
        SceneEditorNodeModel* nodeModel = static_cast<SceneEditorNodeModel*>(node->nodeDataModel());
        const auto nodeInfo = nodeModel->nodeInfo();

        QtNodes::PortIndex portIndex = connection.getPortIndex(portType);
        auto& ports = portType == QtNodes::PortType::In ? nodeInfo->inputs : nodeInfo->outputs;

        // Fill up possibly preceeding nodes
        for(int i = ports.size(); i <= portIndex; i++)
            ports.push_back(std::vector<std::string>());

        ports.at(portIndex).push_back(id);
    }

    Q_EMIT changed();
}

/**
 * @brief SceneEditorScene::onConnectionDeleted
 * @param connection
 */
void SceneEditorScene::onConnectionDeleted(QtNodes::Connection const &connection)
{
    // Connection id
    const std::string id = connection.id().toString(QUuid::WithoutBraces).toStdString();

    for (QtNodes::PortType portType : { QtNodes::PortType::In, QtNodes::PortType::Out })
    {
        // Get the info of each node
        const QtNodes::Node* node = connection.getNode(portType);
        SceneEditorNodeModel* nodeModel = static_cast<SceneEditorNodeModel*>(node->nodeDataModel());
        const auto nodeInfo = nodeModel->nodeInfo();

        const QtNodes::PortIndex portIndex = connection.getPortIndex(portType);
        auto& ports = portType == QtNodes::PortType::In ? nodeInfo->inputs : nodeInfo->outputs;

        // Erase the connection of both nodes
        if (ports.size() > portIndex)
        {
            std::vector<std::string>& ids = ports.at(portIndex);
            ids.erase(std::remove(ids.begin(), ids.end(), id), ids.end());
        }
    }

    Q_EMIT changed();
}

// ### PRIVATE ###

/**
 * @brief SceneEditorScene::addNode
 * @param nodeInfo
 * @return
 */
QtNodes::Node* SceneEditorScene::addNode(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo)
{
    // Create the node
    std::unique_ptr<SceneEditorNodeModel> model = nullptr;
    if (nodeInfo->type == VoukoderPro::NodeInfoType::input)
        model = std::make_unique<SceneEditorInputNodeModel>(nodeInfo, plugins);
    else if (nodeInfo->type == VoukoderPro::NodeInfoType::filter)
        model = std::make_unique<SceneEditorFilterNodeModel>(nodeInfo, plugins);
    else if (nodeInfo->type == VoukoderPro::NodeInfoType::encoder)
        model = std::make_unique<SceneEditorEncoderNodeModel>(nodeInfo, plugins);
    else if (nodeInfo->type == VoukoderPro::NodeInfoType::muxer)
        model = std::make_unique<SceneEditorMuxerNodeModel>(nodeInfo, plugins);
    else if (nodeInfo->type == VoukoderPro::NodeInfoType::output)
        model = std::make_unique<SceneEditorOutputNodeModel>(nodeInfo, plugins);
    else if (nodeInfo->type == VoukoderPro::NodeInfoType::postproc)
        model = std::make_unique<SceneEditorPostProcNodeModel>(nodeInfo, plugins);
    else
        return nullptr;

    model->init();

    if (model)
    {
        connect(model.get(), &SceneEditorNodeModel::changed, this, &SceneEditorScene::changed);

        connect(model.get(), &QtNodes::NodeDataModel::embeddedWidgetSizeUpdated, this, [this]()
        {
            for(const auto& node : connections())
                node.second->getConnectionGraphicsObject().move();

            update();
        });

        auto& node = createNode(std::move(model));

        // Position
        QPointF pos(nodeInfo->posX, nodeInfo->posY);
        node.nodeGraphicsObject().setPos(pos);

        Q_EMIT nodePlaced(node);

        return &node;
    }

    return nullptr;
}
