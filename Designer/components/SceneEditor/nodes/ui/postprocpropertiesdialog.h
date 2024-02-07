#pragma once

#include "propertiesdialog.h"

/**
 * @brief The PostProcPropertiesDialog class
 */
class PostProcPropertiesDialog : public PropertiesDialog
{
public:
    PostProcPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent = nullptr);
};
