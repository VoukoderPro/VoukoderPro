#pragma once

#include <QDialog>
#include <QThread>

#include "../VoukoderPro/voukoderpro_api.h"

class Worker : public QThread
{
    Q_OBJECT
public:
    Worker(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, VoukoderPro::config project);

    void run() override;

signals:
    void message(const QString result);
    void step();

private:
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo;
    VoukoderPro::config project;
};

namespace Ui {
class TestRunDialog;
}

class TestRunDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestRunDialog(std::shared_ptr<VoukoderPro::IClient> vkdrPro, std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, QWidget *parent = nullptr);
    ~TestRunDialog();

private slots:
    void on_startButton_clicked();
    void on_pushButton_3_clicked();

private:
    void start();

private:
    Ui::TestRunDialog *ui;
    std::shared_ptr<VoukoderPro::IClient> vkdrPro;
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo;
};
