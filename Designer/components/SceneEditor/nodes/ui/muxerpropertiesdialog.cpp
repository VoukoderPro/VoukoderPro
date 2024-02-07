#include "muxerpropertiesdialog.h"

/**
 * @brief MuxerPropertiesDialog::MuxerPropertiesDialog
 * @param nodeInfo
 * @param plugins
 * @param parent
 */
MuxerPropertiesDialog::MuxerPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent):
    PropertiesDialog(nodeInfo, plugins, parent)
{
    setWindowTitle(tr("Muxer Properties"));
}
