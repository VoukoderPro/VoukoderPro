#include "scenetestdialog.h"
#include "ui_scenetestdialog.h"

#include "boost/dll/import.hpp"

typedef std::shared_ptr<VoukoderPro::IClient>(pluginapi_create_t)();

Worker::Worker(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, VoukoderPro::config project, const int iterations):
    sceneInfo(sceneInfo), project(project), iterations(iterations)
{}

void Worker::run()
{
    // Create voukoderpro class factory
    auto factory = VOUKODERPRO_CREATE_INSTANCE;
    auto vkdrpro = factory();
    if (!vkdrpro)
        return;

    // Init
    if (vkdrpro->init([&](std::string msg) { emit message(QString::fromStdString(msg)); }) < 0)
        return;

    vkdrpro->setScene(sceneInfo);

    // Open
    if (vkdrpro->open(project) < 0)
        return;

    //int64_t apts = 0;

    // Iterate about all configured frames
    for (int i = 0; i < iterations; ++i)
    {
        // Iterate over all tracks
        for (int t = 0; t < project.tracks.size(); t++)
        {
            const auto& track = project.tracks.at(t);
            const std::string type = std::get<std::string>(track.at(VoukoderPro::pPropType));

            if (type == "video")
            {
                const int width = std::get<int>(track.at(VoukoderPro::pPropWidth));
                const int height = std::get<int>(track.at(VoukoderPro::pPropHeight));
                const size_t size = width * height;

                // Create buffer
                uint8_t* vbuffer[3];
                for (int i = 0; i < sizeof(vbuffer) / sizeof(vbuffer[0]); i++)
                    vbuffer[i] = (uint8_t*)malloc(size);
                int linesize[3] = { 1920, 1024, 1024 };

                /* Y */
                for (int y = 0; y < height; y++)
                    for (int x = 0; x < width; x++)
                        vbuffer[0][y * linesize[0] + x] = x + y + (i % 255) * 3;

                /* Cb and Cr */
                for (int y = 0; y < height / 2; y++) {
                    for (int x = 0; x < width / 2; x++) {
                        vbuffer[1][y * linesize[1] + x] = 128 + y + (i % 255) * 2;
                        vbuffer[2][y * linesize[2] + x] = 64 + x + (i % 255) * 5;
                    }
                }

                // Send frame to voukoder pro
                if (vkdrpro->writeVideoFrame(t, i, vbuffer, linesize) < 0)
                    break;

                // Free memory
                for (int i = 0; i < sizeof(vbuffer) / sizeof(vbuffer[0]); i++)
                    free(vbuffer[i]);
            }
            else if (type == "audio")
            {
                const int channelCount = std::get<int>(track.at(VoukoderPro::pPropChannelCount));

                // Create buffer
                uint8_t* abuffer[2];
                size_t asize = 1024;
                for (int i = 0; i < sizeof(abuffer) / sizeof(abuffer[0]); i++)
                {
                    abuffer[i] = (uint8_t*)malloc(asize);
                    std::memset(abuffer[i], 0, asize);
                }

                // Send samples to voukoder pro
                if (vkdrpro->writeAudioSamples(t, abuffer, static_cast<int>(asize)) < 0)
                    break;

                // Free memory
                for (int i = 0; i < sizeof(abuffer) / sizeof(abuffer[0]); i++)
                    free(abuffer[i]);
            }
            else
            {
                vkdrpro->log("Track type not supported: " + type);
            }
        }
    }

    // Close without saving the performance log
    vkdrpro->close(false);
}

void Worker::stopp()
{

}

SceneTestDialog::SceneTestDialog(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, VoukoderPro::config project, const int iterations, QWidget *parent)
    : QDialog(parent), ui(new Ui::SceneTestDialog), sceneInfo(sceneInfo)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QPalette p = ui->logPanel->palette();
    p.setColor(QPalette::Active, QPalette::Base, Qt::black);
    p.setColor(QPalette::Inactive, QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::lightGray);
    ui->logPanel->setPalette(p);

    // Start test
    worker = new Worker(sceneInfo, project, iterations);
    connect(worker, &Worker::message, this, [&](QString msg){ ui->logPanel->appendPlainText(msg); });
    connect(worker, &Worker::finished, this, [&](){
        ui->logPanel->appendPlainText("Feddisch!");
    });
    connect(worker, &Worker::finished, worker, &QObject::deleteLater);
    worker->start();
}

SceneTestDialog::~SceneTestDialog()
{
    delete ui;
}
