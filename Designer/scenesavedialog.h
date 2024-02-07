#ifndef SCENESAVEDIALOG_H
#define SCENESAVEDIALOG_H

#include <QDialog>
#include <QPushButton>

#include "../VoukoderPro/voukoderpro_api.h"

namespace Ui {
class SceneSaveDialog;
}

class SceneSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SceneSaveDialog(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, QWidget *parent = nullptr);
    ~SceneSaveDialog();

private slots:
    void on_name_textChanged(const QString &arg1);

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SceneSaveDialog *ui;
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo;
};

#endif // SCENESAVEDIALOG_H
