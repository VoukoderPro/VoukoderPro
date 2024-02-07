#include "sceneopendialog.h"
#include "ui_sceneopendialog.h"

#include <QFileInfo>
#include <QPushButton>
#include <QMessageBox>

SceneOpenDialog::SceneOpenDialog(std::shared_ptr<VoukoderPro::ISceneManager> sceneManager, QWidget *parent) :
    QDialog(parent), ui(new Ui::SceneOpenDialog), sceneManager(sceneManager)
{
    ui->setupUi(this);

    // Window flags
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // Load all available Scenes
    if (sceneManager->load(scenes) < 0)
    {
        // error
    }

    for (const auto& scene : scenes)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, QString::fromStdString(scene->name));
        item->setData(0, Qt::UserRole, QVariant::fromValue(scene));

        ui->scenes->addTopLevelItem(item);
    }

    // Select the first item
    if (ui->scenes->topLevelItemCount() > 0)
        ui->scenes->setCurrentItem(ui->scenes->topLevelItem(0));
    else
        ui->buttonBox->button(QDialogButtonBox::Open)->setEnabled(false);

    ui->scenes->setFocus();
}

SceneOpenDialog::~SceneOpenDialog()
{
    delete ui;
}

std::shared_ptr<VoukoderPro::SceneInfo> SceneOpenDialog::selectedSceneInfo()
{
    return ui->scenes->currentItem()->data(0, Qt::UserRole).value<std::shared_ptr<VoukoderPro::SceneInfo>>();
}

void SceneOpenDialog::on_scenes_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    ui->buttonBox->button(QDialogButtonBox::Open)->setEnabled(ui->scenes->selectedItems().count() > 0);
    ui->deleteButton->setEnabled(ui->scenes->selectedItems().count() > 0);
}

void SceneOpenDialog::on_scenes_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->buttonBox->button(QDialogButtonBox::Open)->click();    
}

void SceneOpenDialog::on_deleteButton_clicked()
{
    const QTreeWidgetItem* item = ui->scenes->currentItem();
    if (!item)
        return;

    if (QMessageBox::information(this, tr("Delete scene"), tr("Are you sure you want to delete this scene?"),
                                 QMessageBox::Button::Ok | QMessageBox::Button::Cancel, QMessageBox::Button::Cancel) == QMessageBox::Button::Ok)
    {
        scenes.erase(std::remove_if(scenes.begin(), scenes.end(), [item](std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo)
                       {
                           return sceneInfo == item->data(0, Qt::UserRole).value<std::shared_ptr<VoukoderPro::SceneInfo>>();
                                    }));

        // Save the new scenes
        if (sceneManager->save(scenes) == 0)
            delete item;

        ui->buttonBox->button(QDialogButtonBox::Open)->setEnabled(ui->scenes->selectedItems().count() > 0);
        ui->deleteButton->setEnabled(ui->scenes->selectedItems().count() > 0);
    }

    ui->scenes->setFocus();
}

