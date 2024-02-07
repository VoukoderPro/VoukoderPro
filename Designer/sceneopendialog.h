#ifndef SCENEOPENDIALOG_H
#define SCENEOPENDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

#include "../VoukoderPro/voukoderpro_api.h"

Q_DECLARE_METATYPE(std::shared_ptr<VoukoderPro::SceneInfo>)

namespace Ui {
class SceneOpenDialog;
}

class SceneOpenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SceneOpenDialog(std::shared_ptr<VoukoderPro::ISceneManager> sceneManager, QWidget *parent = nullptr);
    ~SceneOpenDialog();
    std::shared_ptr<VoukoderPro::SceneInfo> selectedSceneInfo();

private slots:
    void on_scenes_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_scenes_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_deleteButton_clicked();

private:
    Ui::SceneOpenDialog *ui;
    std::shared_ptr<VoukoderPro::ISceneManager> sceneManager;
    std::vector<std::shared_ptr<VoukoderPro::SceneInfo>> scenes;
};

#endif // SCENEOPENDIALOG_H
