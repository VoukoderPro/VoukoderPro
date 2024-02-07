#include "filterpropertiesdialog.h"

/**
 * @brief FilterPropertiesDialog::FilterPropertiesDialog
 * @param nodeInfo
 * @param plugins
 * @param parent
 */
FilterPropertiesDialog::FilterPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent):
    PropertiesDialog(nodeInfo, plugins, parent)
{
    setWindowTitle(tr("Filter Properties"));
}
