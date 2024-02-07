#pragma once

#include <QDialog>

#include "../VoukoderPro/types.h"

namespace Ui {
class VideoInputProperties;
}

/**
 * @brief The InputPropertiesDialog class
 */
class InputPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputPropertiesDialog(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo, QWidget *parent = nullptr);
    ~InputPropertiesDialog();
    void getValues(nlohmann::ordered_json& data);

private slots:
    void on_trackStrategy_currentIndexChanged(int index);

private:
    Ui::VideoInputProperties *ui;
};
