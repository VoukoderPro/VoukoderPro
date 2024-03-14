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
    void on_addVTrackButton_clicked();
    void on_editVTrackButton_clicked();
    void on_deleteVTrackButton_clicked();
    void on_videoTracksWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_addATrackButton_clicked();
    void on_editATrackButton_clicked();
    void on_deleteATrackButton_clicked();
    void on_audioTracksWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_tabVideo_currentChanged(int index);
    void on_startButton_clicked();
    void on_frames_editingFinished();

private:
    QTreeWidgetItem* addVideoTrack(QTreeWidget* obj, const int width, const int height, const int timebaseNum, const int timebaseDen, const int aspectNum, const int aspectDen, const std::string fieldOrder, const std::string format, const std::string colorRange, const std::string colorSpace, const std::string colorPrimaries, const std::string colorTransfer);
    QTreeWidgetItem* addAudioTrack(QTreeWidget* obj, const std::string channelLayout, const int sampligRate, const std::string format);
    void load();
    void save();
    void validate();

private:
    Ui::PerformanceTestDialog *ui;
    std::shared_ptr<VoukoderPro::IClient> vkdrPro;
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo;
};

#endif // PERFORMANCETESTDIALOG_H
