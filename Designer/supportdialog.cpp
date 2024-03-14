#include "supportdialog.h"
#include "ui_supportdialog.h"

#include <QDesktopServices>
#include <QMessageBox>

SupportDialog::SupportDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SupportDialog)
{
    ui->setupUi(this);
}

SupportDialog::~SupportDialog()
{
    delete ui;
}

void SupportDialog::on_pushButton_3_clicked()
{
    QString url = "https://www.paypal.com/paypalme/voukoder";
    QDesktopServices::openUrl(url);
}

void SupportDialog::on_pushButton_2_clicked()
{
    QString url = "https://www.patreon.com/voukoder";
    QDesktopServices::openUrl(url);
}

void SupportDialog::on_pushButton_clicked()
{
    QMessageBox::information(this, "VoukoderPro", "To make a SWIFT payment using IBAN and BIC please send an email to daniel@voukoder.org.\nIt is also possible to send you an invoice or receipt if needed.");
}

