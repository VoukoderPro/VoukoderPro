#include "scenetestdialog.h"
#include "ui_scenetestdialog.h"

Worker::Worker(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, VoukoderPro::config project, const int iterations):
    sceneInfo(sceneInfo), project(project), iterations(iterations)
{}

void Worker::run()
{
    // Create voukoderpro class factory
    auto factory = VoukoderProCreateInstance();
    auto vkdrpro = factory();
    if (!vkdrpro)
        return;

    // Init
    if (vkdrpro->init([&](std::string msg){ emit message(QString::fromStdString(msg)); }) < 0)
        return;

    vkdrpro->setScene(sceneInfo);

    // Open
    if (vkdrpro->open(project) < 0)
        return;

    // Iterate about all configured frames
    for (int i = 0; i < iterations; ++i)
    {
        if (stopped)
            break;

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
    stopped = true;
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
    connect(worker, &Worker::finished, this, [=]()
    {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();

        ui->logPanel->appendPlainText("----\nExported " + QString::number(iterations) + " frames in " + QString::number((double)duration / 1000000.0) + " seconds.\nThis is a theoretical average export speed of " + QString::number(1000000.0 / ((double)duration / (double)iterations)) + " fps.\nIMPORTANT! Real exports using an NLE might be slower due to the NLE overhead.");
    });

    start = std::chrono::high_resolution_clock::now();

    worker->start();
}

SceneTestDialog::~SceneTestDialog()
{
    if (worker)
        delete worker;

    delete ui;
}

void SceneTestDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    button->setEnabled(false);

    // Stop worker thread
    worker->stopp();
    worker->wait();
}

