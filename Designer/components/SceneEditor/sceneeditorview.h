#pragma once

#include <nodes/FlowView>
#include <nodes/Node>

#include "sceneeditorscene.h"

/**
 * @brief The SceneEditorView class
 */
class SceneEditorView : public QtNodes::FlowView
{
    Q_OBJECT

public:
    explicit SceneEditorView(SceneEditorScene* scene, QWidget *parent = Q_NULLPTR);
    ~SceneEditorView();

    SceneEditorScene* editorScene();
    QPointF nextNodePos();
    void setNextNodePos(QPointF pos);
    QtNodes::Node* sourceNode();
    void setSourceNode(QtNodes::Node* node);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
    void hideEvent(QHideEvent* event) override {}
    void showEvent(QShowEvent* event) override {}

private Q_SIGNAL:
    void nodeDoubleClick();

public Q_SLOTS:
    void centerScene();

private Q_SLOTS:
    void nodeContextMenuEvent(QtNodes::Node& node, const QPointF& pos);

private:
    void setStyle();

private:
    QPointF _nextNodePos;
    QtNodes::Node* _sourceNode = nullptr;
};
