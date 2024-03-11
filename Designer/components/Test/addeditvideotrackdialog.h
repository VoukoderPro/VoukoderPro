#ifndef ADDEDITVIDEOTRACKDIALOG_H
#define ADDEDITVIDEOTRACKDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class AddEditVideoTrackDialog;
}

class AddEditVideoTrackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditVideoTrackDialog(QWidget *parent = nullptr, QTreeWidgetItem* item = nullptr, const bool editMode = false);
    ~AddEditVideoTrackDialog();
    QTreeWidgetItem* createdItem();

private slots:
    void on_AddEditVideoTrackDialog_accepted();
    void on_AddEditVideoTrackDialog_rejected();

private:
    Ui::AddEditVideoTrackDialog *ui;
    const bool editMode = false;
    QTreeWidgetItem* item = nullptr;
};

#endif // ADDEDITVIDEOTRACKDIALOG_H
