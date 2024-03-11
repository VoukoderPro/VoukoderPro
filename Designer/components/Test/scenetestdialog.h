#ifndef SCENETESTDIALOG_H
#define SCENETESTDIALOG_H

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

private:
    Ui::SceneTestDialog *ui;
    std::shared_ptr<VoukoderPro::IClient> vkdrPro;
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo;
    Worker* worker;
};

#endif // SCENETESTDIALOG_H
