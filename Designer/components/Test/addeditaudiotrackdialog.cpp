#include "addeditaudiotrackdialog.h"
#include "ui_addeditaudiotrackdialog.h"

AddEditAudioTrackDialog::AddEditAudioTrackDialog(QWidget *parent, QTreeWidgetItem* item, const bool editMode)
    : QDialog(parent), ui(new Ui::AddEditAudioTrackDialog), editMode(editMode)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // Channel Layout
    ui->channelLayout->addItem("Mono", QString("mono"));
    ui->channelLayout->addItem("Stereo", QString("stereo"));
    ui->channelLayout->addItem("5.1", QString("5.1"));
    ui->channelLayout->addItem("7.1", QString("7.1"));
    ui->channelLayout->setCurrentText("Stereo");

    // Sample Format
    ui->sampleFormat->addItem("Float", QString("flt"));
    ui->sampleFormat->addItem("Float Planar", QString("fltp"));
    ui->sampleFormat->addItem("16 Bit Integer", QString("s16"));
    ui->sampleFormat->addItem("24 Bit Integer", QString("s24"));
    ui->sampleFormat->addItem("32 Bit Integer", QString("s32"));

    // Set supplied data
    if (item)
    {
        ui->channelLayout->setCurrentIndex(ui->channelLayout->findData(item->text(0)));
        ui->sampleFrequency->setValue(item->text(1).toInt());
        ui->sampleFormat->setCurrentIndex(ui->sampleFormat->findData(item->text(2)));
    }

    if (editMode)
        this->item = item;
    else
        this->item = new QTreeWidgetItem();
}

AddEditAudioTrackDialog::~AddEditAudioTrackDialog()
{
    delete ui;
}

void AddEditAudioTrackDialog::on_AddEditAudioTrackDialog_accepted()
{
    this->item->setText(0, ui->channelLayout->currentData().toString());
    this->item->setText(1, ui->sampleFrequency->text());
    this->item->setText(2, ui->sampleFormat->currentData().toString());
}

void AddEditAudioTrackDialog::on_AddEditAudioTrackDialog_rejected()
{
    if (!editMode)
        delete this->item;
}

QTreeWidgetItem* AddEditAudioTrackDialog::createdItem()
{
    return this->item;
}
