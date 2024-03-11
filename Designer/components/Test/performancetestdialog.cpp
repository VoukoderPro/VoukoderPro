#include "performancetestdialog.h"
#include "ui_performancetestdialog.h"

#include <QMessageBox>

#include "components/Test/addeditvideotrackdialog.h"
#include "components/Test/addeditaudiotrackdialog.h"
#include "components/Test/scenetestdialog.h"

PerformanceTestDialog::PerformanceTestDialog(std::shared_ptr<VoukoderPro::IClient> vkdrPro, std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerformanceTestDialog), vkdrPro(vkdrPro), sceneInfo(sceneInfo)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // Video tracks
    ui->videoTracksWidget->setColumnWidth(0, 75);
    ui->videoTracksWidget->setColumnWidth(1, 75);
    ui->videoTracksWidget->setColumnWidth(2, 75);
    ui->videoTracksWidget->setColumnWidth(3, 75);
    ui->videoTracksWidget->setColumnWidth(4, 100);
    ui->videoTracksWidget->setColumnWidth(5, 100);
    ui->videoTracksWidget->setColumnWidth(6, 100);
    ui->videoTracksWidget->setColumnWidth(7, 100);
    ui->videoTracksWidget->setColumnWidth(8, 100);
    ui->videoTracksWidget->setColumnWidth(9, 100);

    // Audio tracks
    ui->audioTracksWidget->setColumnWidth(0, 150);
    ui->audioTracksWidget->setColumnWidth(1, 150);
    ui->audioTracksWidget->setColumnWidth(2, 150);

    //addVideoTrack(ui->videoTracksWidget, 1920, 1080, 1, 30, 1, 1, "progressive", "yuv420p", "tv", "bt709", "bt709", "bt709", "00:00:00:00", "eng");
    //addAudioTrack(ui->audioTracksWidget, "FL+FR", 44100, "fltp");

    validate();
}

PerformanceTestDialog::~PerformanceTestDialog()
{
    delete ui;
}

QTreeWidgetItem* PerformanceTestDialog::addVideoTrack(QTreeWidget* parent, const int width, const int height, const int timebaseNum, const int timebaseDen,
                                          const int aspectNum, const int aspectDen, const std::string fieldOrder, const std::string format,
                                          const std::string colorRange, const std::string colorSpace, const std::string colorPrimaries, const std::string colorTransfer,
                                          const std::string timecode, const std::string language)
{
    QTreeWidgetItem* track = new QTreeWidgetItem(ui->videoTracksWidget);
    track->setText(0, QString::number(width));
    track->setText(1, QString::number(height));
    track->setText(2, QString("%1/%2").arg(timebaseNum).arg(timebaseDen));
    track->setText(3, QString("%1:%2").arg(aspectNum).arg(aspectDen));
    track->setText(4, QString::fromStdString(fieldOrder));
    track->setText(5, QString::fromStdString(format));
    track->setText(6, QString::fromStdString(colorRange));
    track->setText(7, QString::fromStdString(colorSpace));
    track->setText(8, QString::fromStdString(colorPrimaries));
    track->setText(9, QString::fromStdString(colorTransfer));

    parent->addTopLevelItem(track);

    // const QList<QTreeWidgetItem*> selectedItems = parent->selectedItems();
    // if (selectedItems.size() == 1)
    // {
    //     parent->setCurrentItem(track);
    //     parent->setSortingEnabled(false);
    // }

    return track;
}

QTreeWidgetItem* PerformanceTestDialog::addAudioTrack(QTreeWidget* parent, const std::string channelLayout, const int sampligRate, const std::string format)
{
    QTreeWidgetItem* track = new QTreeWidgetItem(ui->audioTracksWidget);
    track->setText(0, QString::fromStdString(channelLayout));
    track->setText(1, QString::number(sampligRate));
    track->setText(2, QString::fromStdString(format));

    parent->addTopLevelItem(track);

    // const QList<QTreeWidgetItem*> selectedItems = parent->selectedItems();
    // if (selectedItems.size() == 1)
    // {
    //     parent->setCurrentItem(track);
    //     parent->setSortingEnabled(false);
    // }

    return track;
}

void PerformanceTestDialog::on_closeButton_clicked()
{
    close();
}

void PerformanceTestDialog::validate()
{
    const QList<QTreeWidgetItem*> vSelectedItems = ui->videoTracksWidget->selectedItems();
    ui->editVTrackButton->setEnabled(vSelectedItems.size() > 0);
    ui->deleteVTrackButton->setEnabled(vSelectedItems.size() > 0);

    const QList<QTreeWidgetItem*> aSelectedItems = ui->audioTracksWidget->selectedItems();
    ui->editATrackButton->setEnabled(aSelectedItems.size() > 0);
    ui->deleteATrackButton->setEnabled(aSelectedItems.size() > 0);

    ui->startButton->setEnabled(ui->videoTracksWidget->topLevelItemCount() + ui->audioTracksWidget->topLevelItemCount() > 0);
}

void PerformanceTestDialog::on_addVTrackButton_clicked()
{
    QTreeWidgetItem* item = nullptr;
    if (ui->videoTracksWidget->selectedItems().size() > 0)
        item = ui->videoTracksWidget->selectedItems().front();

    AddEditVideoTrackDialog dialog(this, item);
    dialog.setWindowTitle("Add Video Track");
    if (dialog.exec() == DialogCode::Accepted)
    {
        QTreeWidgetItem* item = dialog.createdItem();
        ui->videoTracksWidget->addTopLevelItem(item);
        ui->videoTracksWidget->setCurrentItem(item);
        ui->videoTracksWidget->setFocus();

        validate();
    }
}

void PerformanceTestDialog::on_editVTrackButton_clicked()
{
    if (ui->videoTracksWidget->selectedItems().size() > 0)
    {
        AddEditVideoTrackDialog dialog(this, ui->videoTracksWidget->currentItem(), true);
        dialog.setWindowTitle("Edit Video Track");
        dialog.exec();

        ui->videoTracksWidget->setFocus();

        validate();
    }
}

void PerformanceTestDialog::on_deleteVTrackButton_clicked()
{
    if (QMessageBox::question(this, "VoukoderPro", "Are you sure you want to delete this track?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        delete ui->videoTracksWidget->currentItem();

        validate();
    }
}

void PerformanceTestDialog::on_videoTracksWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    on_editVTrackButton_clicked();
}

void PerformanceTestDialog::on_addATrackButton_clicked()
{
    QTreeWidgetItem* item = nullptr;
    if (ui->audioTracksWidget->selectedItems().size() > 0)
        item = ui->audioTracksWidget->selectedItems().front();

    AddEditAudioTrackDialog dialog(this, item);
    dialog.setWindowTitle("Add Audio Track");
    if (dialog.exec() == DialogCode::Accepted)
    {
        QTreeWidgetItem* item = dialog.createdItem();
        ui->audioTracksWidget->addTopLevelItem(item);
        ui->audioTracksWidget->setCurrentItem(item);
        ui->audioTracksWidget->setFocus();

        validate();
    }
}

void PerformanceTestDialog::on_editATrackButton_clicked()
{
    if (ui->audioTracksWidget->selectedItems().size() > 0)
    {
        AddEditAudioTrackDialog dialog(this, ui->audioTracksWidget->currentItem(), true);
        dialog.setWindowTitle("Edit Audio Track");
        dialog.exec();

        ui->audioTracksWidget->setFocus();

        validate();
    }
}

void PerformanceTestDialog::on_deleteATrackButton_clicked()
{
    if (QMessageBox::question(this, "VoukoderPro", "Are you sure you want to delete this track?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        delete ui->audioTracksWidget->currentItem();

        validate();
    }
}

void PerformanceTestDialog::on_audioTracksWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    on_editATrackButton_clicked();
}

void PerformanceTestDialog::on_tabVideo_currentChanged(int index)
{
    if (index == 0)
        ui->videoTracksWidget->setFocus();
    else if (index == 1)
        ui->audioTracksWidget->setFocus();
}

void PerformanceTestDialog::on_startButton_clicked()
{
    // Project structure
    VoukoderPro::config project;
    project[VoukoderPro::pPropApplication] = QApplication::applicationName().toStdString();
    project[VoukoderPro::pPropFilename] = "NUL";
    project[VoukoderPro::pPropConfiguration] = "";

    // Video track(s)
    for (int i = 0; i < ui->videoTracksWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* item = ui->videoTracksWidget->topLevelItem(i);

        VoukoderPro::config videoTrack;
        videoTrack[VoukoderPro::pPropType] = "video";
        videoTrack[VoukoderPro::pPropWidth] = item->text(0).toInt();
        videoTrack[VoukoderPro::pPropHeight] = item->text(1).toInt();

        const QStringList timebase = item->text(2).split('/');
        videoTrack[VoukoderPro::pPropTimebaseNum] = timebase.at(0).toInt();
        videoTrack[VoukoderPro::pPropTimebaseDen] = timebase.at(1).toInt();

        const QStringList aspect = item->text(3).split(':');
        videoTrack[VoukoderPro::pPropAspectNum] = aspect.at(0).toInt();
        videoTrack[VoukoderPro::pPropAspectDen] = aspect.at(1).toInt();

        videoTrack[VoukoderPro::pPropFieldOrder] = item->text(4).toStdString();
        videoTrack[VoukoderPro::pPropFormat] = item->text(5).toStdString();
        videoTrack[VoukoderPro::pPropColorRange] = item->text(6).toStdString();
        videoTrack[VoukoderPro::pPropColorSpace] = item->text(7).toStdString();
        videoTrack[VoukoderPro::pPropColorPrimaries] = item->text(8).toStdString();
        videoTrack[VoukoderPro::pPropColorTransfer] = item->text(9).toStdString();
        project.tracks.push_back(videoTrack);
    }

    // Audio track(s)
    for (int i = 0; i < ui->audioTracksWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* item = ui->audioTracksWidget->topLevelItem(i);

        VoukoderPro::config audioTrack;
        audioTrack[VoukoderPro::pPropType] = "audio";

        const QString layout = item->text(0);
        audioTrack[VoukoderPro::pPropChannelCount] = (int)layout.split("+").size();
        audioTrack[VoukoderPro::pPropChannelLayout] = layout.toStdString();

        audioTrack[VoukoderPro::pPropSamplingRate] = item->text(1).toInt();
        audioTrack[VoukoderPro::pPropFormat] = item->text(2).toStdString();
        project.tracks.push_back(audioTrack);
    }

    // Start test
    SceneTestDialog dialog(sceneInfo, project, ui->frames->value(), this);
    dialog.exec();
}

