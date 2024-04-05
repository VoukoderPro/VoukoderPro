#ifndef PERFORMANCETESTDIALOG_H
#define PERFORMANCETESTDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

#include "../VoukoderPro/voukoderpro_api.h"

namespace Ui {
class PerformanceTestDialog;
}

class PerformanceTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PerformanceTestDialog(std::shared_ptr<VoukoderPro::IClient> vkdrPro, std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, QWidget *parent = nullptr);
    ~PerformanceTestDialog();

private slots:
    void on_closeButton_clicked();

private:
    QTreeWidgetItem* addVideoTrack(const int width, const int height, const int timebaseNum, const int timebaseDen, const int aspectNum, const int aspectDen, const std::string fieldOrder, const std::string format, const std::string colorRange, const std::string colorSpace, const std::string colorPrimaries, const std::string colorTransfer, const std::string timecode, const std::string language);
    QTreeWidgetItem* addAudioTrack(const std::string channelLayout, const int sampligRate, const std::string format);

private:
    Ui::PerformanceTestDialog *ui;
    std::shared_ptr<VoukoderPro::IClient> vkdrPro;
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo;
};

#endif // PERFORMANCETESTDIALOG_H