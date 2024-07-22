#include "inputpropertiesdialog.h"

#include <QRegularExpression>

#include "ui_inputpropertiesdialog.h"

/**
 * @brief InputPropertiesDialog::InputPropertiesDialog
 * @param nodeInfo
 * @param parent
 */
InputPropertiesDialog::InputPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoInputProperties)
{
    ui->setupUi(this);

    // Window flags
    setWindowFlag(Qt::CustomizeWindowHint, true);
    setWindowFlag(Qt::WindowTitleHint, true);
    setWindowFlag(Qt::WindowSystemMenuHint, false);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    // Track validator
    QRegularExpression regexp ("^([0-9]*)+(,[0-9]+)+$");
    ui->lineEdit->setValidator(new QRegularExpressionValidator(regexp, this));

    auto& data = nodeInfo->data;

    if (nodeInfo->mediaType == VoukoderPro::MediaType::video)
    {
        setWindowTitle(tr("Video input properties"));

        ui->trackStrategy->addItem(tr("Use for all video tracks"));
        ui->trackStrategy->addItem(tr("Specific video tracks"));

        // Populate Color Matrix
        ui->colorMatrix->addItem(tr("(Auto)"), QString("auto"));
        ui->colorMatrix->addItem("BT.470bg", QString("bt470bg"));
        ui->colorMatrix->addItem("BT.709", QString("bt709"));
        ui->colorMatrix->addItem("BT.2020nc", QString("bt2020nc"));
        ui->colorMatrix->addItem("BT.2020c", QString("bt2020c"));
        ui->colorMatrix->addItem("SMPTE 170m", QString("smpte170m"));
        ui->colorMatrix->addItem("SMPTE 240m", QString("smpte240m"));
        ui->colorMatrix->addItem("SMPTE 2085", QString("smpte2085"));
        ui->colorMatrix->addItem("FCC", QString("fcc"));
        ui->colorMatrix->addItem("GBR", QString("gbr"));
        ui->colorMatrix->addItem("YCgCo", QString("YCgCo"));
        ui->colorMatrix->addItem("Chroma derived nc", QString("chroma-derived-nc"));
        ui->colorMatrix->addItem("Chroma derived c", QString("chroma-derived-c"));
        ui->colorMatrix->addItem("ICTCP", QString("ictcp"));

        // Populate Color Matrix
        ui->colorPrimaries->addItem(tr("(Auto)"), QString("auto"));
        ui->colorPrimaries->addItem("BT.470m", QString("bt470m"));
        ui->colorPrimaries->addItem("BT.470bg", QString("bt470bg"));
        ui->colorPrimaries->addItem("BT.709", QString("bt709"));
        ui->colorPrimaries->addItem("BT.2020", QString("bt2020"));
        ui->colorPrimaries->addItem("Film", QString("film"));
        ui->colorPrimaries->addItem("SMPTE 170m", QString("smpte170m"));
        ui->colorPrimaries->addItem("SMPTE 240m", QString("smpte240m"));
        ui->colorPrimaries->addItem("SMPTE 428", QString("smpte428"));
        ui->colorPrimaries->addItem("SMPTE 431", QString("smpte431"));
        ui->colorPrimaries->addItem("SMPTE 432", QString("smpte432"));

        // Populate Color Transfer
        ui->colorTransfer->addItem(tr("(Auto)"), QString("auto"));
        ui->colorTransfer->addItem("BT.470m", QString("bt470m"));
        ui->colorTransfer->addItem("BT.470bg", QString("bt470bg"));
        ui->colorTransfer->addItem("BT.709", QString("bt709"));
        ui->colorTransfer->addItem("BT.1361e", QString("bt1361e"));
        ui->colorTransfer->addItem("BT.2020-10", QString("bt2020-10"));
        ui->colorTransfer->addItem("BT.2020-12", QString("bt2020-12"));
        ui->colorTransfer->addItem("SMPTE 170m", QString("smpte170m"));
        ui->colorTransfer->addItem("SMPTE 240m", QString("smpte240m"));
        ui->colorTransfer->addItem("SMPTE 428", QString("smpte428"));
        ui->colorTransfer->addItem("SMPTE 2084", QString("smpte2084"));
        ui->colorTransfer->addItem("Linear", QString("linear"));
        ui->colorTransfer->addItem("Log 100", QString("log100"));
        ui->colorTransfer->addItem("Log 316", QString("log316"));
        ui->colorTransfer->addItem("IEC 61966-2-1", QString("iec61966-2-1"));
        ui->colorTransfer->addItem("IEC 61966-2-4", QString("iec61966-2-4"));
        ui->colorTransfer->addItem("ARIB STD-B67", QString("arib-std-b67"));

        if (data.contains("color") && data["color"].is_object())
        {
            nlohmann::ordered_json& color = data["color"];

            const QString range = QString::fromStdString(color.contains("range") && color["range"].is_string() ? color["range"].get<std::string>() : "auto");
            if (range == "tv")
                ui->colorRangeLimited->setChecked(true);
            else if (range == "pc")
                ui->colorRangeFull->setChecked(true);
            else
                ui->colorRangeAuto->setChecked(true);

            const QString matrix = QString::fromStdString(color.contains("matrix") && color["matrix"].is_string() ? color["matrix"].get<std::string>() : "auto");
            ui->colorMatrix->setCurrentIndex(ui->colorMatrix->findData(matrix));

            const QString primaries = QString::fromStdString(color.contains("primaries") && color["primaries"].is_string() ? color["primaries"].get<std::string>() : "auto");
            ui->colorPrimaries->setCurrentIndex(ui->colorPrimaries->findData(primaries));

            const QString transfer = QString::fromStdString(color.contains("transfer") && color["transfer"].is_string() ? color["transfer"].get<std::string>() : "auto");
            ui->colorTransfer->setCurrentIndex(ui->colorTransfer->findData(transfer));
        }
    }
    else
    {
        setWindowTitle(tr("Audio input properties"));
        ui->tabWidget->setTabVisible(ui->tabWidget->indexOf(ui->colors), false);

        ui->trackStrategy->addItem(tr("Use for all audio tracks"));
        ui->trackStrategy->addItem(tr("Specific audio tracks"));
    }

    // Track mapping
    if (data.contains("tracks") && data["tracks"].is_array() && data["tracks"].size() > 0)
    {
        QStringList tracks;
        for (const auto& track : data["tracks"])
            tracks.append(QString::number(track.get<int>()));

        ui->trackStrategy->setCurrentIndex(1);
        ui->lineEdit->setText(tracks.join(','));
    }
    else
    {
        ui->trackStrategy->setCurrentIndex(0);
        ui->lineEdit->setText("");
    }
}

/**
 * @brief InputPropertiesDialog::~InputPropertiesDialog
 */
InputPropertiesDialog::~InputPropertiesDialog()
{
    delete ui;
}

/**
 * @brief InputPropertiesDialog::getValues
 * @param data
 */
void InputPropertiesDialog::getValues(nlohmann::ordered_json& data)
{
    // Track mapping
    data["tracks"] = nlohmann::ordered_json::array();
    if (ui->trackStrategy->currentIndex() == 1)
    {
        const QString tracks = ui->lineEdit->text();
        const QStringList trackList = tracks.split(',', Qt::SkipEmptyParts);

        for (QString track : trackList)
            data["tracks"].push_back(track.toUInt());
    }

    // Color
    QString range = "auto";
    if (ui->colorRangeFull->isChecked())
        range = "pc";
    else if (ui->colorRangeLimited->isChecked())
        range = "tv";

    data["color"] = {
        { "range", range.toStdString() },
        { "matrix", ui->colorMatrix->currentData().toString().toStdString() },
        { "primaries", ui->colorPrimaries->currentData().toString().toStdString() },
        { "transfer", ui->colorTransfer->currentData().toString().toStdString() }
    };
}

/**
 * @brief InputPropertiesDialog::on_trackStrategy_currentIndexChanged
 * @param index
 */
void InputPropertiesDialog::on_trackStrategy_currentIndexChanged(int index)
{
    ui->lineEdit->setEnabled(index == 1);
    if (index == 1)
        ui->lineEdit->setFocus();
}
