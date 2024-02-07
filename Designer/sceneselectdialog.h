#ifndef SCENESELECTDIALOG_H
#define SCENESELECTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include "../VoukoderPro/voukoderpro_api.h"

namespace Ui {
class SceneSelectDialog;
}

class SceneSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SceneSelectDialog(std::shared_ptr<VoukoderPro::IClient> vkdrPro, const QString name, QWidget *parent = nullptr);
    ~SceneSelectDialog();

    QString selectedScene() const;

private Q_SLOTS:
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);

private:
    Ui::SceneSelectDialog *ui;
    std::shared_ptr<VoukoderPro::IClient> vkdrPro;
};

#endif // SCENESELECTDIALOG_H
