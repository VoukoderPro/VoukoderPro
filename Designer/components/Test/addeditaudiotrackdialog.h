#ifndef ADDEDITAUDIOTRACKDIALOG_H
#define ADDEDITAUDIOTRACKDIALOG_H

#include <vector>
#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class AddEditAudioTrackDialog;
}

class AddEditAudioTrackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditAudioTrackDialog(QWidget *parent = nullptr, QTreeWidgetItem* item = nullptr, const bool editMode = false);
    ~AddEditAudioTrackDialog();
    QTreeWidgetItem* createdItem();

private slots:
    void on_AddEditAudioTrackDialog_accepted();
    void on_AddEditAudioTrackDialog_rejected();

private:
    QString layouts();

private:
    Ui::AddEditAudioTrackDialog *ui;
    const bool editMode = false;
    QTreeWidgetItem* item = nullptr;
};

#endif // ADDEDITAUDIOTRACKDIALOG_H
