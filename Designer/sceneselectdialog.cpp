#include "sceneselectdialog.h"
#include "ui_sceneselectdialog.h"
#include "mainwindow.h"

#include <QTimer>

SceneSelectDialog::SceneSelectDialog(std::shared_ptr<VoukoderPro::IClient> vkdrPro, const QString name, QWidget *parent) :
    QDialog(parent), ui(new Ui::SceneSelectDialog), vkdrPro(vkdrPro)
{
    ui->setupUi(this);

    // Window flags
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint );

    std::vector<std::shared_ptr<VoukoderPro::SceneInfo>> infos;
    vkdrPro->sceneManager()->load(infos);

    // Populate all items
    for (const auto& info : infos)
        ui->listWidget->addItem(QString::fromStdString(info->name));

    // Can we preselect one item?
    if (ui->listWidget->count() > 0)
    {
        QList<QListWidgetItem*> items = ui->listWidget->findItems(name, Qt::MatchExactly);
        ui->listWidget->setCurrentItem(items.size() > 0 ? items.at(0) : ui->listWidget->item(0));
    }
}

SceneSelectDialog::~SceneSelectDialog()
{
    delete ui;
}

QString SceneSelectDialog::selectedScene() const
{
    return ui->listWidget->selectedItems().at(0)->text();
}

void SceneSelectDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    accept();
}
