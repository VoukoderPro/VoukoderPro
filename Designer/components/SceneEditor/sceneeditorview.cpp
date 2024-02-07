#include "sceneeditorview.h"

#include "../../mainwindow.h"
#include <nodes/Node>
#include <nodes/Connection>
#include <nodes/internal/ConnectionStyle.hpp>
#include <QContextMenuEvent>
#include <nodes/FlowScene>
#include <QGraphicsItem>
#include <QMenu>

/**
 * @brief SceneEditorView::SceneEditorView
 * @param scene
 * @param parent
 */
SceneEditorView::SceneEditorView(SceneEditorScene* scene, QWidget *parent): QtNodes::FlowView(scene, parent)
{
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setStyle();

    // Events
    connect(scene, &QtNodes::FlowScene::nodeContextMenu, this, &SceneEditorView::nodeContextMenuEvent);

    connect(scene, &QtNodes::FlowScene::nodeDeleted, this, [&](QtNodes::Node& node)
    {
        if (&node == _sourceNode)
        {
            _sourceNode = nullptr;
            _nextNodePos = {0, 0};
        }
    });
}

/**
 * @brief SceneEditorView::~SceneEditorView
 */
SceneEditorView::~SceneEditorView()
{
    if (scene())
        delete scene();
}

/**
 * @brief SceneEditorView::nextNodePos
 * @return
 */
QPointF SceneEditorView::nextNodePos()
{
    return _nextNodePos;
}

/**
 * @brief SceneEditorView::setNextNodePos
 * @param pos
 */
void SceneEditorView::setNextNodePos(QPointF pos)
{
    _nextNodePos = pos;
}

/**
 * @brief SceneEditorView::sourceNode
 * @return
 */
QtNodes::Node* SceneEditorView::sourceNode()
{
    return _sourceNode;
}

/**
 * @brief SceneEditorView::setSourceNode
 * @param node
 */
void SceneEditorView::setSourceNode(QtNodes::Node* node)
{
    _sourceNode = node;
}

/**
 * @brief SceneEditorView::centerScene
 */
void SceneEditorView::centerScene()
{
    if (scene())
    {
        setSceneRect ({});
        QRectF r = scene()->itemsBoundingRect();
        //QRect rect(mapToScene(0,0).toPoint(), viewport()->rect().size());
        centerOn(r.center());
    }
}

/**
 * @brief SceneEditorView::editorScene
 * @return
 */
SceneEditorScene* SceneEditorView::editorScene()
{
    return static_cast<SceneEditorScene*>(scene());
}

/**
 * @brief SceneEditorView::wheelEvent
 * @param event
 */
void SceneEditorView::wheelEvent(QWheelEvent *event)
{}

/**
 * @brief SceneEditorView::contextMenuEvent
 * @param event
 */
void SceneEditorView::contextMenuEvent(QContextMenuEvent* event)
{
    // Did we click on a node?
    if (itemAt(event->pos()))
    {
        QGraphicsView::contextMenuEvent(event);
        return;
    }

    // Show the node menu as context menu
    auto window = static_cast<MainWindow*>(topLevelWidget());
    if (window)
    {
        QPointF pos = mapToScene(event->pos());

        _sourceNode = nullptr;
        _nextNodePos.setY(pos.y());
        _nextNodePos.setX(pos.x());

        QMenu* menu = window->nodeMenu();
        menu->exec(event->globalPos());
    }
}

/**
 * @brief SceneEditorView::nodeContextMenuEvent
 * @param node
 * @param pos
 */
void SceneEditorView::nodeContextMenuEvent(QtNodes::Node& node, const QPointF& pos)
{
    // Translate to screen position
    QPoint viewPos = mapFromScene(pos);

    _sourceNode = &node;

    QtNodes::NodeGraphicsObject* ngo = static_cast<QtNodes::NodeGraphicsObject*>(itemAt(viewPos));
    if (ngo)
    {
        _nextNodePos.setY(ngo->y());
        _nextNodePos.setX(ngo->x() + ngo->boundingRect().width());
    }

    auto screenPos = viewport()->mapToGlobal(viewPos);

    // Show the node menu as context menu
    auto window = static_cast<MainWindow*>(topLevelWidget());
    if (window)
    {
        QMenu* menu = window->nodeMenu(node);
        menu->exec(screenPos);
    }
}

/**
 * @brief SceneEditorView::setStyle
 */
void SceneEditorView::setStyle()
{
//    QtNodes::NodeStyle::setNodeStyle(R"(
//        {
//          "NodeStyle": {
//            "NormalBoundaryColor": [80, 80, 80],
//            "SelectedBoundaryColor": [240, 240, 240],
//            "GradientColor0": [80, 80, 80],
//            "GradientColor1": [80, 80, 80],
//            "GradientColor2": [80, 80, 80],
//            "GradientColor3": [80, 80, 80],
//            "ShadowColor": [0, 0, 0],
//            "FontColor": [240, 240, 240],
//            "FontColorFaded": [100, 100, 100],
//            "ConnectionPointColor": "white",
//            "PenWidth": 2.0,
//            "HoveredPenWidth": 2.5,
//            "ConnectionPointDiameter": 10.0,
//            "Opacity": 1.0
//          }
//        }
//    )");

//    QtNodes::ConnectionStyle::setConnectionStyle(R"(
//        {
//          "ConnectionStyle": {
//            "ConstructionColor": "gray",
//            "NormalColor": "black",
//            "SelectedColor": "gray",
//            "SelectedHaloColor": "deepskyblue",
//            "HoveredColor": "deepskyblue",
//            "LineWidth": 3.0,
//            "ConstructionLineWidth": 2.0,
//            "PointDiameter": 10.0,
//            "UseDataDefinedColors": true
//          }
//        }
//    )");

    QtNodes::ConnectionStyle::setConnectionStyle(R"(
        {
          "ConnectionStyle": {
            "UseDataDefinedColors": true
          }
        }
    )");
}
