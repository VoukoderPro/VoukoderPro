#pragma once

#include <QVariantMap>
#include <nodes/NodeData>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

#define VPRO_DATA_CODEC_ID "codecId"
#define VPRO_DATA_ENCODER_NAME "encoderName"

/**
 * @brief The SceneEditorNodeData class
 */
class SceneEditorNodeData : public NodeData
{
public:
    QVariantMap store;

    NodeDataType type() const override
    {
        return NodeDataType {"track", "Track"};
    }
};
