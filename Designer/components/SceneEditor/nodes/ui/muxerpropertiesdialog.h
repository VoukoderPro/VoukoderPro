#pragma once

#include "propertiesdialog.h"

/**
 * @brief The MuxerPropertiesDialog class
 */
class MuxerPropertiesDialog : public PropertiesDialog
{
public:
    MuxerPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent = nullptr);
};
