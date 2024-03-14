#ifndef SUPPORTDIALOG_H
#define SUPPORTDIALOG_H

#include <QDialog>

namespace Ui {
class SupportDialog;
}

class SupportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SupportDialog(QWidget *parent = nullptr);
    ~SupportDialog();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SupportDialog *ui;
};

#endif // SUPPORTDIALOG_H
