#include "outputpropertiesdialog.h"
#include "ui_propertiesdialog.h"
#include <boost/url/url.hpp>

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
        if (url == "${OutputFile.Absolute}")
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
        data["url"] = ui->filenameGroup->isChecked() ? ui->filenameInput->text().toStdString() : "${OutputFile.Absolute}";
    else if (data["params"].is_object())
    {
        boost::urls::url url;
        url.set_scheme(data["id"].get<std::string>());

        for (const auto& [key, value] : data["params"].items())
        {
            if (value.is_string())
            {
                const std::string val = value.get<std::string>();

                if (key == "_host")
                {
                    url.set_host(val);
                    continue;
                }
            }
            else if (value.is_number())
            {
                const int val = value.get<int>();

                if (key == "_port")
                {
                    url.set_port(std::to_string(val));
                    continue;
                }
            }
        }

        data["url"] = url.c_str();
    }
}
