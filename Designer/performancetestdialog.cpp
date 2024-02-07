#include "performancetestdialog.h"
#include "ui_performancetestdialog.h"

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

    QTreeWidgetItem* videoTrack = addVideoTrack(1920, 1080, 1, 30, 1, 1, "progressive", "yuv420p", "tv", "bt709", "bt709", "bt709", "00:00:00:00", "eng");
    videoTrack->setSelected(true);
    ui->videoTracksWidget->setCurrentItem(videoTrack);
    ui->videoTracksWidget->setFocus();

    QTreeWidgetItem* audioTrack = addAudioTrack("FL+FR", 44100, "fltp");
    ui->audioTracksWidget->setCurrentItem(audioTrack);
    ui->audioTracksWidget->setFocus();
}

PerformanceTestDialog::~PerformanceTestDialog()
{
    delete ui;
}

QTreeWidgetItem* PerformanceTestDialog::addVideoTrack(const int width, const int height, const int timebaseNum, const int timebaseDen,
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

    ui->videoTracksWidget->addTopLevelItem(track);

    return track;
}

QTreeWidgetItem* PerformanceTestDialog::addAudioTrack(const std::string channelLayout, const int sampligRate, const std::string format)
{
    QTreeWidgetItem* track = new QTreeWidgetItem(ui->audioTracksWidget);
    track->setText(0, QString::fromStdString(channelLayout));
    track->setText(1, QString::number(sampligRate));
    track->setText(2, QString::fromStdString(format));

    ui->audioTracksWidget->addTopLevelItem(track);

    return track;
}

void PerformanceTestDialog::on_closeButton_clicked()
{
    close();
}
