#include "outputpropertiesdialog.h"
#include "ui_propertiesdialog.h"

/**
 * @brief OutputPropertiesDialog::OutputPropertiesDialog
 * @param nodeInfo
 * @param plugins
 * @param parent
 */
OutputPropertiesDialog::OutputPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, const std::vector<VoukoderPro::AssetInfo>& plugins, QWidget *parent):
    PropertiesDialog(nodeInfo, plugins, parent)
{
    setWindowTitle(tr("Output Properties"));

    // Only proceed if the "output" and "params.url" parameters are set
    if (!nodeInfo->data.contains("id") || !nodeInfo->data["id"].is_string() ||
            !nodeInfo->data.contains("url") || !nodeInfo->data["url"].is_string())
        return;

    const QString url = QString::fromStdString(nodeInfo->data["url"].get<std::string>());

    if (nodeInfo->data["id"].get<std::string>() == "file")
    {
        if (url == "$(OutputFilename)")
        {
            ui->filenameGroup->setChecked(false);
            ui->filenameInput->setText("");
        }
        else
        {
            ui->filenameGroup->setChecked(true);
            ui->filenameInput->setText(url);
        }
    }
    else
        ui->urlInput->setText(url);
}

/**
 * @brief OutputPropertiesDialog::getValues
 * @param data
 */
void OutputPropertiesDialog::getValues(nlohmann::ordered_json& data)
{
    // Handle standard values
    PropertiesDialog::getValues(data);

    // Set output
    if (data["id"] == "file")
    {
        data["url"] = ui->filenameGroup->isChecked() ? ui->filenameInput->text().toStdString() : "$(OutputFilename)";
    }
    else
    {
        data["url"] = ui->urlInput->text().toStdString();
    }
}
