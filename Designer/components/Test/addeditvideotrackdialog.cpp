#include "addeditvideotrackdialog.h"
#include "ui_addeditvideotrackdialog.h"

AddEditVideoTrackDialog::AddEditVideoTrackDialog(QWidget *parent, QTreeWidgetItem* item, const bool editMode)
    : QDialog(parent), ui(new Ui::AddEditVideoTrackDialog), editMode(editMode)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // Populate field order
    ui->fieldOrder->addItem("Progressive", QString("progressive"));
    ui->fieldOrder->addItem("Top Field First", QString("tff"));
    ui->fieldOrder->addItem("Bottom Field First", QString("bff"));

    // Populate Color Format
    ui->colorFormat->addItem("YUV 4:2:0 (8 Bit)", QString("yuv420p"));
    ui->colorFormat->addItem("YUV 4:2:2 (8 Bit)", QString("yuv422p"));
    ui->colorFormat->addItem("YUV 4:4:4 (8 Bit)", QString("yuv444p"));
    ui->colorFormat->addItem("YUV 4:2:0 (10 Bit)", QString("yuv420p10le"));
    ui->colorFormat->addItem("YUV 4:2:2 (10 Bit)", QString("yuv422p10le"));
    ui->colorFormat->addItem("YUV 4:4:4 (10 Bit)", QString("yuv444p10le"));
    ui->colorFormat->addItem("YUV 4:2:0 (12 Bit)", QString("yuv420p12le"));
    ui->colorFormat->addItem("YUV 4:2:2 (12 Bit)", QString("yuv422p12le"));
    ui->colorFormat->addItem("YUV 4:4:4 (12 Bit)", QString("yuv444p12le"));
    ui->colorFormat->addItem("YUV 4:2:0 (14 Bit)", QString("yuv420p14le"));
    ui->colorFormat->addItem("YUV 4:2:2 (14 Bit)", QString("yuv422p14le"));
    ui->colorFormat->addItem("YUV 4:4:4 (14 Bit)", QString("yuv444p14le"));
    ui->colorFormat->addItem("YUV 4:2:0 (16 Bit)", QString("yuv420p16le"));
    ui->colorFormat->addItem("YUV 4:2:2 (16 Bit)", QString("yuv422p16le"));
    ui->colorFormat->addItem("YUV 4:4:4 (16 Bit)", QString("yuv444p16le"));

    // Populate Color Range
    ui->colorRange->addItem("TV / Limited", QString("tv"));
    ui->colorRange->addItem("PC / Full", QString("pc"));

    // Populate Color Matrix
    ui->colorSpace->addItem("BT.470bg", QString("bt470bg"));
    ui->colorSpace->addItem("BT.709", QString("bt709"));
    ui->colorSpace->addItem("BT.2020nc", QString("bt2020nc"));
    ui->colorSpace->addItem("BT.2020c", QString("bt2020c"));
    ui->colorSpace->addItem("SMPTE 170m", QString("smpte170m"));
    ui->colorSpace->addItem("SMPTE 240m", QString("smpte240m"));
    ui->colorSpace->addItem("SMPTE 2085", QString("smpte2085"));
    ui->colorSpace->addItem("FCC", QString("fcc"));
    ui->colorSpace->addItem("GBR", QString("gbr"));
    ui->colorSpace->addItem("YCgCo", QString("YCgCo"));
    ui->colorSpace->addItem("Chroma derived nc", QString("chroma-derived-nc"));
    ui->colorSpace->addItem("Chroma derived c", QString("chroma-derived-c"));
    ui->colorSpace->addItem("ICTCP", QString("ictcp"));
    ui->colorSpace->setCurrentText("BT.709");

    // Populate Color Matrix
    ui->colorPrimaries->addItem("BT.470m", QString("bt470m"));
    ui->colorPrimaries->addItem("BT.470bg", QString("bt470bg"));
    ui->colorPrimaries->addItem("BT.709", QString("bt709"));
    ui->colorPrimaries->addItem("BT.2020", QString("bt2020"));
    ui->colorPrimaries->addItem("Film", QString("film"));
    ui->colorPrimaries->addItem("SMPTE 170m", QString("smpte170m"));
    ui->colorPrimaries->addItem("SMPTE 240m", QString("smpte240m"));
    ui->colorPrimaries->addItem("SMPTE 428", QString("smpte428"));
    ui->colorPrimaries->addItem("SMPTE 431", QString("smpte431"));
    ui->colorPrimaries->addItem("SMPTE 432", QString("smpte432"));
    ui->colorPrimaries->setCurrentText("BT.709");

    // Populate Color Transfer
    ui->colorTransfer->addItem("BT.470m", QString("bt470m"));
    ui->colorTransfer->addItem("BT.470bg", QString("bt470bg"));
    ui->colorTransfer->addItem("BT.709", QString("bt709"));
    ui->colorTransfer->addItem("BT.1361e", QString("bt1361e"));
    ui->colorTransfer->addItem("BT.2020-10", QString("bt2020-10"));
    ui->colorTransfer->addItem("BT.2020-12", QString("bt2020-12"));
    ui->colorTransfer->addItem("SMPTE 170m", QString("smpte170m"));
    ui->colorTransfer->addItem("SMPTE 240m", QString("smpte240m"));
    ui->colorTransfer->addItem("SMPTE 428", QString("smpte428"));
    ui->colorTransfer->addItem("SMPTE 2084", QString("smpte2084"));
    ui->colorTransfer->addItem("Linear", QString("linear"));
    ui->colorTransfer->addItem("Log 100", QString("log100"));
    ui->colorTransfer->addItem("Log 316", QString("log316"));
    ui->colorTransfer->addItem("IEC 61966-2-1", QString("iec61966-2-1"));
    ui->colorTransfer->addItem("IEC 61966-2-4", QString("iec61966-2-4"));
    ui->colorTransfer->addItem("ARIB STD-B67", QString("arib-std-b67"));
    ui->colorTransfer->setCurrentText("BT.709");

    // Set supplied data
    if (item)
    {
        // Frame size
        ui->width->setValue(item->text(0).toInt());
        ui->height->setValue(item->text(1).toInt());

        // Timebase
        QStringList timebase = item->text(2).split("/");
        if (timebase.size() == 2)
        {
            ui->timebaseNum->setValue(timebase.at(0).toInt());
            ui->timebaseDen->setValue(timebase.at(1).toInt());
        }

        // Pixel Aspect Ratio
        QStringList aspect = item->text(3).split(":");
        if (aspect.size() == 2)
        {
            ui->aspectNum->setValue(aspect.at(0).toInt());
            ui->aspectDen->setValue(aspect.at(1).toInt());
        }

        // Dropdowns
        ui->fieldOrder->setCurrentIndex(ui->fieldOrder->findData(item->text(4)));
        ui->colorFormat->setCurrentIndex(ui->colorFormat->findData(item->text(5)));
        ui->colorRange->setCurrentIndex(ui->colorRange->findData(item->text(6)));
        ui->colorSpace->setCurrentIndex(ui->colorSpace->findData(item->text(7)));
        ui->colorPrimaries->setCurrentIndex(ui->colorPrimaries->findData(item->text(8)));
        ui->colorTransfer->setCurrentIndex(ui->colorTransfer->findData(item->text(9)));
    }

    if (editMode)
        this->item = item;
    else
        this->item = new QTreeWidgetItem();
}

AddEditVideoTrackDialog::~AddEditVideoTrackDialog()
{
    delete ui;
}

void AddEditVideoTrackDialog::on_AddEditVideoTrackDialog_accepted()
{
    this->item->setText(0, ui->width->text());
    this->item->setText(1, ui->height->text());
    this->item->setText(2, ui->timebaseNum->text() + "/" + ui->timebaseDen->text());
    this->item->setText(3, ui->aspectNum->text() + ":" + ui->aspectDen->text());
    this->item->setText(4, ui->fieldOrder->currentData().toString());
    this->item->setText(5, ui->colorFormat->currentData().toString());
    this->item->setText(6, ui->colorRange->currentData().toString());
    this->item->setText(7, ui->colorSpace->currentData().toString());
    this->item->setText(8, ui->colorPrimaries->currentData().toString());
    this->item->setText(9, ui->colorTransfer->currentData().toString());
}

void AddEditVideoTrackDialog::on_AddEditVideoTrackDialog_rejected()
{
    if (!editMode)
        delete this->item;
}

QTreeWidgetItem* AddEditVideoTrackDialog::createdItem()
{
    return this->item;
}
