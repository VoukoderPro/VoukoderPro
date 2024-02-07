#include "scenesavedialog.h"
#include "ui_scenesavedialog.h"

SceneSaveDialog::SceneSaveDialog(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, QWidget *parent) :
    QDialog(parent), ui(new Ui::SceneSaveDialog), sceneInfo(sceneInfo)
{
    ui->setupUi(this);

    // Window flags
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    ui->name->setText(QString::fromStdString(sceneInfo->name));
    ui->name->selectAll();

    on_name_textChanged(ui->name->text());
}

SceneSaveDialog::~SceneSaveDialog()
{
    delete ui;
}

void SceneSaveDialog::on_name_textChanged(const QString &text)
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(text.length() > 0);
}

void SceneSaveDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button == ui->buttonBox->button(QDialogButtonBox::Save))
        sceneInfo->name = ui->name->text().toStdString();
}
