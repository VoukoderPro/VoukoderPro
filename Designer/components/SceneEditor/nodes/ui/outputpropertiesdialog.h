#pragma once

#include "propertiesdialog.h"

/**
 * @brief The OutputPropertiesDialog class
 */
class OutputPropertiesDialog : public PropertiesDialog
{
public:
    OutputPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent = nullptr);
    void getValues(nlohmann::ordered_json& data);
};
