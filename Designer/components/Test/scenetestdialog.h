#ifndef SCENETESTDIALOG_H
#define SCENETESTDIALOG_H

#include <chrono>

#include <QDialog>
#include <QThread>
#include <QAbstractButton>

#include "../VoukoderPro/voukoderpro_api.h"

class Worker : public QThread
{
    Q_OBJECT
public:
    Worker(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, VoukoderPro::config project, const int iterations);
    void run() override;
    void stopp();

signals:
    void message(const QString result);

private:
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo;
    VoukoderPro::config project;
    const int iterations;
    bool stopped = false;
};

namespace Ui {
    class SceneTestDialog;
}

class SceneTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SceneTestDialog(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, VoukoderPro::config project, const int iterations, QWidget *parent = nullptr);
    ~SceneTestDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SceneTestDialog *ui;
    std::shared_ptr<VoukoderPro::IClient> vkdrPro;
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo;
    std::chrono::high_resolution_clock::time_point start;
    Worker* worker;
};

#endif // SCENETESTDIALOG_H
