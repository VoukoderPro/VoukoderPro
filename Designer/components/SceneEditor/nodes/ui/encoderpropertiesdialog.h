#pragma once

#include "propertiesdialog.h"

/**
 * @brief The EncoderPropertiesDialog class
 */
class EncoderPropertiesDialog : public PropertiesDialog
{
public:
    EncoderPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent = nullptr);
    void getValues(nlohmann::ordered_json& data);

private:
    void loadSideData(nlohmann::ordered_json& sidedata);
};
