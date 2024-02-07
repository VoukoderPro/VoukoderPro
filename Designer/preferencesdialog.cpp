#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::init(std::vector<VoukoderPro::AssetInfo> plugins)
{
    // On-Start scene
    ui->openScene->addItem("(" + tr("None") + ")", "none");
    ui->openScene->addItem(tr("Empty"), "empty");
    ui->openScene->addItem(tr("Simple"), "simple");

    // Sort the plugins alphabetically
    std::sort(plugins.begin(), plugins.end(), [](VoukoderPro::AssetInfo const& a, VoukoderPro::AssetInfo const& b)
              {
                  return a.name < b.name;
              });

    // Populate dropdown boxes
    for (const VoukoderPro::AssetInfo& pluginInfo : plugins)
    {
        const QString id = QString::fromStdString(pluginInfo.id);
        const QString name = QString::fromStdString(pluginInfo.name);

        if (pluginInfo.type == VoukoderPro::NodeInfoType::filter)
        {
            if (pluginInfo.mediaType == VoukoderPro::MediaType::video)
                ui->filterVideo->addItem(name, id);
            else if (pluginInfo.mediaType == VoukoderPro::MediaType::audio)
                ui->filterAudio->addItem(name, id);
        }
        else if (pluginInfo.type == VoukoderPro::NodeInfoType::encoder)
        {
            if (pluginInfo.mediaType == VoukoderPro::MediaType::video)
                ui->encoderVideo->addItem(name, id);
            else if (pluginInfo.mediaType == VoukoderPro::MediaType::audio)
                ui->encoderAudio->addItem(name, id);
        }
        else if (pluginInfo.type == VoukoderPro::NodeInfoType::muxer)
            ui->muxer->addItem(name, id);
        else if (pluginInfo.type == VoukoderPro::NodeInfoType::output)
            ui->output->addItem(name, id);
        else if (pluginInfo.type == VoukoderPro::NodeInfoType::postproc)
            ui->postproc->addItem(name, id);
    }


    // Pre-select the current configuration
    ui->openScene->setCurrentIndex(qMax(0, ui->openScene->findData(QString::fromStdString(prefs.get<std::string>(VPRO_GENERAL_OPEN_SCENE)))));
    ui->filterVideo->setCurrentIndex(qMax(0, ui->filterVideo->findData(QString::fromStdString(prefs.get<std::string>(VPRO_DEFAULT_VIDEO_FILTER)))));
    ui->filterAudio->setCurrentIndex(qMax(0, ui->filterAudio->findData(QString::fromStdString(prefs.get<std::string>(VPRO_DEFAULT_AUDIO_FILTER)))));
    ui->encoderVideo->setCurrentIndex(qMax(0, ui->encoderVideo->findData(QString::fromStdString(prefs.get<std::string>(VPRO_DEFAULT_VIDEO_ENCODER)))));
    ui->encoderAudio->setCurrentIndex(qMax(0, ui->encoderAudio->findData(QString::fromStdString(prefs.get<std::string>(VPRO_DEFAULT_AUDIO_ENCODER)))));
    ui->muxer->setCurrentIndex(qMax(0, ui->muxer->findData(QString::fromStdString(prefs.get<std::string>(VPRO_DEFAULT_MUXER)))));
    ui->output->setCurrentIndex(qMax(0, ui->output->findData(QString::fromStdString(prefs.get<std::string>(VPRO_DEFAULT_OUTPUT)))));
    ui->postproc->setCurrentIndex(qMax(0, ui->output->findData(QString::fromStdString(prefs.get<std::string>(VPRO_DEFAULT_POSTPROC)))));
    ui->techNames->setCheckState(prefs.get<bool>(VPRO_PROPERTIES_TECHNAMES) ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}

void PreferencesDialog::on_buttonBox_accepted()
{
    prefs.set(VPRO_GENERAL_OPEN_SCENE, ui->openScene->currentData().value<QString>().toStdString());
    prefs.set(VPRO_DEFAULT_VIDEO_FILTER, ui->filterVideo->currentData().value<QString>().toStdString());
    prefs.set(VPRO_DEFAULT_AUDIO_FILTER, ui->filterAudio->currentData().value<QString>().toStdString());
    prefs.set(VPRO_DEFAULT_VIDEO_ENCODER, ui->encoderVideo->currentData().value<QString>().toStdString());
    prefs.set(VPRO_DEFAULT_AUDIO_ENCODER, ui->encoderAudio->currentData().value<QString>().toStdString());
    prefs.set(VPRO_DEFAULT_MUXER, ui->muxer->currentData().value<QString>().toStdString());
    prefs.set(VPRO_DEFAULT_OUTPUT, ui->output->currentData().value<QString>().toStdString());
    prefs.set(VPRO_DEFAULT_POSTPROC, ui->postproc->currentData().value<QString>().toStdString());
    prefs.set(VPRO_PROPERTIES_TECHNAMES, ui->techNames->isChecked());

    prefs.save();
}
