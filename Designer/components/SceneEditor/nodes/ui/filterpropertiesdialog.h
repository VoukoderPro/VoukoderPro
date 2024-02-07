#pragma once

#include "propertiesdialog.h"

/**
 * @brief The FilterPropertiesDialog class
 */
class FilterPropertiesDialog : public PropertiesDialog
{
public:
    FilterPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent = nullptr);
};
