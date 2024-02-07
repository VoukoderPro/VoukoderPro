#pragma once

#include <QDialog>

#include "../VoukoderPro/voukoderpro_api.h"
#include "preferences.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog();

    void init(std::vector<VoukoderPro::AssetInfo> plugins);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PreferencesDialog *ui;
    std::vector<VoukoderPro::AssetInfo> plugins;
    Preferences& prefs = Preferences::instance();
};
