#include "testrundialog.h"
#include "boost/dll/import.hpp"
#include "ui_testrundialog.h"

//#include <functional>
//#include <boost/shared_ptr.hpp>
//#include <boost/dll/alias.hpp>
#include <boost/filesystem.hpp>

typedef std::shared_ptr<VoukoderPro::IClient>(pluginapi_create_t)();

static const int TEST_FRAMES = 1;

static void fill_yuv_image(uint8_t* vbuffer[3], int linesize[3], int frame_index, int width, int height)
{
    int x, y, i;

    i = frame_index;

    /* Y */
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            vbuffer[0][y * linesize[0] + x] = x + y + i * 3;

    /* Cb and Cr */
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            vbuffer[1][y * linesize[1] + x] = 128 + y + i * 2;
            vbuffer[2][y * linesize[2] + x] = 64 + x + i * 5;
        }
    }
}

Worker::Worker(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, VoukoderPro::config project):
    sceneInfo(sceneInfo), project(project)
{}

void Worker::run()
{
    Q_EMIT step();

    // Create voukoderpro class factory
    auto factory = VOUKODERPRO_CREATE_INSTANCE;
    auto vkdrpro = factory();
    if (!vkdrpro)
        return;

    Q_EMIT step();

    // Init
    if (vkdrpro->init([&](std::string msg) { emit message(QString::fromStdString(msg)); }) < 0)
        return;

    Q_EMIT step();

    vkdrpro->setScene(sceneInfo);

    // Open
    if (vkdrpro->open(project) < 0)
        return;

    Q_EMIT step();

    const auto videoTrack = project.tracks.at(0);
    const auto audioTrack = project.tracks.at(1);

    size_t size = std::get<int>(videoTrack.at(VoukoderPro::pPropWidth)) * std::get<int>(videoTrack.at(VoukoderPro::pPropHeight));
    uint8_t* vbuffer[3];
    for (int i = 0; i < sizeof(vbuffer) / sizeof(vbuffer[0]); i++)
        vbuffer[i] = (uint8_t*)malloc(size);
    int linesize[3] = { 1920, 1024, 1024 };

    uint8_t* abuffer[2];
    size_t asize = 1024;
    for (int i = 0; i < sizeof(abuffer) / sizeof(abuffer[0]); i++)
    {
        abuffer[i] = (uint8_t*)malloc(asize);
        std::memset(abuffer[i], 0, asize);
    }

    int64_t apts = 0;
    for (int i = 0; i < TEST_FRAMES; ++i)
    {
        fill_yuv_image(vbuffer, linesize, i, std::get<int>(videoTrack.at(VoukoderPro::pPropWidth)), std::get<int>(videoTrack.at(VoukoderPro::pPropHeight)));

        if (vkdrpro->writeVideoFrame(0, i, vbuffer, linesize) < 0)
            break;

        while (true)
        {
            if (((i + 1) * std::get<int>(audioTrack.at(VoukoderPro::pPropSamplingRate)) - std::get<int>(videoTrack.at(VoukoderPro::pPropTimebaseDen)) * apts) <= 0)
                break;

            if (vkdrpro->writeAudioSamples(1, abuffer, static_cast<int>(asize)) < 0)
                break;

            apts += asize / 4;
        }

        Q_EMIT step();
    }

    for (int i = 0; i < sizeof(vbuffer) / sizeof(vbuffer[0]); i++)
        free(vbuffer[i]);

    for (int i = 0; i < sizeof(abuffer) / sizeof(abuffer[0]); i++)
        free(abuffer[i]);

    // Close without saving the performance log
    vkdrpro->close(false);

    Q_EMIT step();

    Q_EMIT step();
}

TestRunDialog::TestRunDialog(std::shared_ptr<VoukoderPro::IClient> vkdrPro, std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestRunDialog), vkdrPro(vkdrPro), sceneInfo(sceneInfo)
{
    ui->setupUi(this);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(6 + TEST_FRAMES);
    ui->progressBar->setValue(0);

    QPalette p = ui->logPanel->palette();
    p.setColor(QPalette::Active, QPalette::Base, Qt::black);
    p.setColor(QPalette::Inactive, QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::lightGray);
    ui->logPanel->setPalette(p);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

TestRunDialog::~TestRunDialog()
{
    delete ui;
}

void TestRunDialog::start()
{
    ui->logPanel->clear();
    ui->startButton->setEnabled(false);

    // Project structure
    VoukoderPro::config project;
    project[VoukoderPro::pPropApplication] = QApplication::applicationName().toStdString();
    project[VoukoderPro::pPropFilename] = "NUL";
    project[VoukoderPro::pPropConfiguration] = "";

    // Video track
    VoukoderPro::config videoTrack;
    videoTrack[VoukoderPro::pPropType] = "video";
    videoTrack[VoukoderPro::pPropWidth] = 1920;
    videoTrack[VoukoderPro::pPropHeight] = 1080;
    videoTrack[VoukoderPro::pPropTimebaseNum] = 1;
    videoTrack[VoukoderPro::pPropTimebaseDen] = 30;
    videoTrack[VoukoderPro::pPropAspectNum] = 1;
    videoTrack[VoukoderPro::pPropAspectDen] = 1;
    videoTrack[VoukoderPro::pPropFieldOrder] = "progressive";
    videoTrack[VoukoderPro::pPropFormat] = "yuv420p";
    videoTrack[VoukoderPro::pPropColorRange] = "tv";
    videoTrack[VoukoderPro::pPropColorSpace] = "bt709";
    videoTrack[VoukoderPro::pPropColorPrimaries] = "bt709";
    videoTrack[VoukoderPro::pPropColorTransfer] = "bt709";
    videoTrack[VoukoderPro::pPropTimecode] = "00:00:00:00";
    videoTrack[VoukoderPro::pPropLanguage] = "eng";
    project.tracks.push_back(videoTrack);

    // Audio track
    VoukoderPro::config audioTrack;
    audioTrack[VoukoderPro::pPropType] = "audio";
    audioTrack[VoukoderPro::pPropChannelCount] = 2;
    audioTrack[VoukoderPro::pPropSamplingRate] = 44100;
    audioTrack[VoukoderPro::pPropChannelLayout] = "FL+FR";
    audioTrack[VoukoderPro::pPropFormat] = "fltp";
    audioTrack[VoukoderPro::pPropLanguage] = "eng";
    project.tracks.push_back(audioTrack);

    ui->progressBar->setValue(0);

    // Start test
    Worker* worker = new Worker(sceneInfo, project);
    connect(worker, &Worker::message, this, [&](QString msg){ ui->logPanel->appendPlainText(msg); });
    connect(worker, &Worker::step, this, [&](){ ui->progressBar->setValue(ui->progressBar->value() + 1); });
    connect(worker, &Worker::finished, this, [&](){
        ui->startButton->setEnabled(true);
    });
    connect(worker, &Worker::finished, worker, &QObject::deleteLater);
    worker->start();
}

void TestRunDialog::on_startButton_clicked()
{
    start();
}

void TestRunDialog::on_pushButton_3_clicked()
{
    close();
}
