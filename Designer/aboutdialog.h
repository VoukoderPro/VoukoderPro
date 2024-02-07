#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "../VoukoderPro/voukoderpro_api.h"

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(std::shared_ptr<VoukoderPro::IClient> vkdrPro, QWidget *parent = nullptr);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
