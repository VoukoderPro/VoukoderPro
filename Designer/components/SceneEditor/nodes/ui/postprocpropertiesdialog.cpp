#include "postprocpropertiesdialog.h"

/**
 * @brief PostProcPropertiesDialog::PostProcPropertiesDialog
 * @param nodeInfo
 * @param plugins
 * @param parent
 */
PostProcPropertiesDialog::PostProcPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent):
    PropertiesDialog(nodeInfo, plugins, parent)
{
    setWindowTitle(tr("Post Processing Properties"));
}
