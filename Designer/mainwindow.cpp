#include "mainwindow.h"

#include "components/SceneEditor/nodes/sceneeditornodemodel.h"
#include <nodes/internal/NodeGraphicsObject.hpp>
#include <nodes/internal/Node.hpp>
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "preferencesdialog.h"
#include "testrundialog.h"
#include "sceneopendialog.h"
#include "scenesavedialog.h"
#include "preferences.h"
#include "newsdialog.h"
#include "performancetestdialog.h"
#include "../VoukoderPro/Version.h"

#include <QTimer>
#include <QUuid>
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include <QJsonDocument>
#include <QStandardPaths>

#include <boost/filesystem.hpp>

MainWindow::MainWindow(std::shared_ptr<VoukoderPro::IClient> vkdrPro, const QString name, QWidget *parent): QMainWindow(parent),
    ui(new Ui::MainWindow), vkdrPro(vkdrPro), sceneMgr(vkdrPro->sceneManager())
{
    ui->setupUi(this);

    setWindowTitle(APP_NAME);
    setCursor(Qt::WaitCursor);

    plugins = vkdrPro->plugins();

    prefs.load();

    newsDialog = new NewsDialog(this);
    connect(newsDialog, &NewsDialog::unreadNewsAvailable, this, &MainWindow::onNewsUpdate);

    //NewsDialog::latestNewsDate(NEWS_CHANNEL_BETA);

    // Load the initial scene
    if (!name.isEmpty())
    {
        if (QFileInfo::exists(name))
        {
            std::shared_ptr<VoukoderPro::SceneInfo> scene;
            if (sceneMgr->importScene(scene, name.toStdString()) == 0)
                addSceneTab(scene);
        }
        else
        {
            std::vector<std::shared_ptr<VoukoderPro::SceneInfo>> scenes;
            if (sceneMgr->load(scenes) == 0)
            {
                auto i = std::find_if(scenes.begin(), scenes.end(), [name](std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo) {
                    return sceneInfo->name == name.toStdString();
                });
                if (i != scenes.end())
                    addSceneTab(*i);
            }
        }
    }
    else
    {
        const std::string openScene = prefs.get<std::string>(VPRO_GENERAL_OPEN_SCENE);
        if (openScene == "empty")
            on_actionSceneNewEmpty_triggered();
        else if (openScene == "simple")
            on_actionSceneNewSimple_triggered();
    }

    setCursor(Qt::ArrowCursor);
}

MainWindow::~MainWindow()
{
    // Close all scenes and delete its views
    for (int index = 0; index < ui->scenes->count(); index++)
    {
        QWidget* widget = ui->scenes->widget(index);

        ui->scenes->removeTab(index);

        delete widget;
    }

    delete ui;
}

// ### FILE ###

void MainWindow::on_actionSceneNewEmpty_triggered()
{
    // Create a new scene
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo = std::make_shared<VoukoderPro::SceneInfo>();
    sceneInfo->name = "";
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo = std::make_shared<VoukoderPro::NodeInfo>();

    addSceneTab(sceneInfo);
}

void MainWindow::on_actionSceneNewSimple_triggered()
{
    Preferences& prefs = Preferences::instance();

    // Create a new scene
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo = std::make_shared<VoukoderPro::SceneInfo>();
    sceneInfo->name = "";

    // Add input video node
    std::shared_ptr<VoukoderPro::NodeInfo> videoInputNode = std::make_shared<VoukoderPro::NodeInfo>();
    videoInputNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    videoInputNode->type = VoukoderPro::NodeInfoType::input;
    videoInputNode->mediaType = VoukoderPro::MediaType::video;
    videoInputNode->posX = -100.0;
    videoInputNode->posY = -78.0;
    videoInputNode->data = { { "tracks", nlohmann::ordered_json::array() } };
    videoInputNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    sceneInfo->nodes.push_back(videoInputNode);

    // Add input audio node
    std::shared_ptr<VoukoderPro::NodeInfo> audioInputNode = std::make_shared<VoukoderPro::NodeInfo>();
    audioInputNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    audioInputNode->type = VoukoderPro::NodeInfoType::input;
    audioInputNode->mediaType = VoukoderPro::MediaType::audio;
    audioInputNode->posX = -100.0;
    audioInputNode->posY = 32.0;
    audioInputNode->data = { { "tracks", nlohmann::ordered_json::array() } };
    audioInputNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    sceneInfo->nodes.push_back(audioInputNode);

    // Add video encoder node
    std::shared_ptr<VoukoderPro::NodeInfo> videoEncoderNode = std::make_shared<VoukoderPro::NodeInfo>();
    videoEncoderNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    videoEncoderNode->type = VoukoderPro::NodeInfoType::encoder;
    videoEncoderNode->mediaType = VoukoderPro::MediaType::video;
    videoEncoderNode->posX = 90.0;
    videoEncoderNode->posY = -78.0;
    videoEncoderNode->inputs.push_back({ videoInputNode->outputs.at(0).at(0) });
    videoEncoderNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    videoEncoderNode->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_VIDEO_ENCODER) } };
    sceneInfo->nodes.push_back(videoEncoderNode);

    // Add audio encoder node
    std::shared_ptr<VoukoderPro::NodeInfo> audioEncoderNode = std::make_shared<VoukoderPro::NodeInfo>();
    audioEncoderNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    audioEncoderNode->type = VoukoderPro::NodeInfoType::encoder;
    audioEncoderNode->mediaType = VoukoderPro::MediaType::audio;
    audioEncoderNode->posX = 90.0;
    audioEncoderNode->posY = 32.0;
    audioEncoderNode->inputs.push_back({ audioInputNode->outputs.at(0).at(0) });
    audioEncoderNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    audioEncoderNode->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_AUDIO_ENCODER) } };
    sceneInfo->nodes.push_back(audioEncoderNode);

    // Add muxer node
    std::shared_ptr<VoukoderPro::NodeInfo> muxerNode = std::make_shared<VoukoderPro::NodeInfo>();
    muxerNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    muxerNode->type = VoukoderPro::NodeInfoType::muxer;
    muxerNode->mediaType = VoukoderPro::MediaType::mux;
    muxerNode->posX = 330.0;
    muxerNode->posY = -40.0;
    muxerNode->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_MUXER) } };
    muxerNode->inputs.push_back({ videoEncoderNode->outputs.at(0).at(0) });
    muxerNode->inputs.push_back({ audioEncoderNode->outputs.at(0).at(0) });
    muxerNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    sceneInfo->nodes.push_back(muxerNode);

    // Add output node
    std::shared_ptr<VoukoderPro::NodeInfo> outputNode = std::make_shared<VoukoderPro::NodeInfo>();
    outputNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    outputNode->type = VoukoderPro::NodeInfoType::output;
    outputNode->mediaType = VoukoderPro::MediaType::mux;
    outputNode->posX = 530.0;
    outputNode->posY = -40.0;
    outputNode->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_OUTPUT) }, { "url", "$(OutputFilename)" } };
    outputNode->inputs.push_back({ muxerNode->outputs.at(0).at(0) });
    sceneInfo->nodes.push_back(outputNode);

    addSceneTab(sceneInfo);
}

void MainWindow::on_actionSceneNewFilter_triggered()
{
    Preferences& prefs = Preferences::instance();

    // Create a new scene
    std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo = std::make_shared<VoukoderPro::SceneInfo>();
    sceneInfo->name = "";

    // Add input video node
    std::shared_ptr<VoukoderPro::NodeInfo> videoInputNode = std::make_shared<VoukoderPro::NodeInfo>();
    videoInputNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    videoInputNode->type = VoukoderPro::NodeInfoType::input;
    videoInputNode->mediaType = VoukoderPro::MediaType::video;
    videoInputNode->posX = -300.0;
    videoInputNode->posY = -78.0;
    videoInputNode->data = { { "tracks", nlohmann::ordered_json::array() } };
    videoInputNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    sceneInfo->nodes.push_back(videoInputNode);

    // Add input audio node
    std::shared_ptr<VoukoderPro::NodeInfo> audioInputNode = std::make_shared<VoukoderPro::NodeInfo>();
    audioInputNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    audioInputNode->type = VoukoderPro::NodeInfoType::input;
    audioInputNode->mediaType = VoukoderPro::MediaType::audio;
    audioInputNode->posX = -300.0;
    audioInputNode->posY = 32.0;
    audioInputNode->data = { { "tracks", nlohmann::ordered_json::array() } };
    audioInputNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    sceneInfo->nodes.push_back(audioInputNode);

    // Add video filter node
    std::shared_ptr<VoukoderPro::NodeInfo> videoFilterNode = std::make_shared<VoukoderPro::NodeInfo>();
    videoFilterNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    videoFilterNode->type = VoukoderPro::NodeInfoType::filter;
    videoFilterNode->mediaType = VoukoderPro::MediaType::video;
    videoFilterNode->posX = -115.0;
    videoFilterNode->posY = -78.0;
    videoFilterNode->inputs.push_back({ videoInputNode->outputs.at(0).at(0) });
    videoFilterNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    videoFilterNode->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_VIDEO_FILTER) } };
    sceneInfo->nodes.push_back(videoFilterNode);

    // Add audio filter node
    std::shared_ptr<VoukoderPro::NodeInfo> audioFilterNode = std::make_shared<VoukoderPro::NodeInfo>();
    audioFilterNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    audioFilterNode->type = VoukoderPro::NodeInfoType::filter;
    audioFilterNode->mediaType = VoukoderPro::MediaType::audio;
    audioFilterNode->posX = -115.0;
    audioFilterNode->posY = 32.0;
    audioFilterNode->inputs.push_back({ audioInputNode->outputs.at(0).at(0) });
    audioFilterNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    audioFilterNode->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_AUDIO_FILTER) } };
    sceneInfo->nodes.push_back(audioFilterNode);

    // Add video encoder node
    std::shared_ptr<VoukoderPro::NodeInfo> videoEncoderNode = std::make_shared<VoukoderPro::NodeInfo>();
    videoEncoderNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    videoEncoderNode->type = VoukoderPro::NodeInfoType::encoder;
    videoEncoderNode->mediaType = VoukoderPro::MediaType::video;
    videoEncoderNode->posX = 90.0;
    videoEncoderNode->posY = -78.0;
    videoEncoderNode->inputs.push_back({ videoFilterNode->outputs.at(0).at(0) });
    videoEncoderNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    videoEncoderNode->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_VIDEO_ENCODER) } };
    sceneInfo->nodes.push_back(videoEncoderNode);

    // Add audio encoder node
    std::shared_ptr<VoukoderPro::NodeInfo> audioEncoderNode = std::make_shared<VoukoderPro::NodeInfo>();
    audioEncoderNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    audioEncoderNode->type = VoukoderPro::NodeInfoType::encoder;
    audioEncoderNode->mediaType = VoukoderPro::MediaType::audio;
    audioEncoderNode->posX = 90.0;
    audioEncoderNode->posY = 32.0;
    audioEncoderNode->inputs.push_back({ audioFilterNode->outputs.at(0).at(0) });
    audioEncoderNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    audioEncoderNode->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_AUDIO_ENCODER) } };
    sceneInfo->nodes.push_back(audioEncoderNode);

    // Add muxer node
    std::shared_ptr<VoukoderPro::NodeInfo> muxerNode = std::make_shared<VoukoderPro::NodeInfo>();
    muxerNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    muxerNode->type = VoukoderPro::NodeInfoType::muxer;
    muxerNode->mediaType = VoukoderPro::MediaType::mux;
    muxerNode->posX = 330.0;
    muxerNode->posY = -40.0;
    muxerNode->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_MUXER) } };
    muxerNode->inputs.push_back({ videoEncoderNode->outputs.at(0).at(0) });
    muxerNode->inputs.push_back({ audioEncoderNode->outputs.at(0).at(0) });
    muxerNode->outputs.push_back({ QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString() });
    sceneInfo->nodes.push_back(muxerNode);

    // Add output node
    std::shared_ptr<VoukoderPro::NodeInfo> outputNode = std::make_shared<VoukoderPro::NodeInfo>();
    outputNode->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    outputNode->type = VoukoderPro::NodeInfoType::output;
    outputNode->mediaType = VoukoderPro::MediaType::mux;
    outputNode->posX = 530.0;
    outputNode->posY = -40.0;
    outputNode->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_OUTPUT) }, { "url", "$(OutputFilename)" } };
    outputNode->inputs.push_back({ muxerNode->outputs.at(0).at(0) });
    sceneInfo->nodes.push_back(outputNode);

    addSceneTab(sceneInfo);
}

void MainWindow::on_actionSceneOpen_triggered()
{
    SceneOpenDialog dialog(vkdrPro->sceneManager(), this);
    if (dialog.exec() == QDialog::Accepted)
        addSceneTab(dialog.selectedSceneInfo());
}

void MainWindow::on_actionSceneSave_triggered()
{
    SceneEditorView* view = currentSceneView();
    if (view)
    {
        SceneEditorScene* scene = view->editorScene();
        if (scene->isModified())
        {
            auto sceneInfo = scene->sceneInfo();
            if (sceneInfo->name.empty())
                on_actionSceneSaveAs_triggered();
            else
            {
                if (saveScene(sceneInfo, true))
                    scene->markNotModified();
            }
        }
    }
}

void MainWindow::on_actionSceneSaveAs_triggered()
{
    SceneEditorView* view = currentSceneView();
    if (view)
    {
        SceneEditorScene* scene = view->editorScene();
        std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo = scene->sceneInfo();

        SceneSaveDialog dialog(sceneInfo, this);
        if (dialog.exec() == QDialog::Accepted)
        {
            const int idx = ui->scenes->currentIndex();
            ui->scenes->setTabText(idx, QString::fromStdString(sceneInfo->name));

            if (saveScene(sceneInfo))
                scene->markNotModified();
        }
    }
}

void MainWindow::on_actionSceneClose_triggered()
{
    const int current = ui->scenes->currentIndex();
    if (current > -1)
        closeScene(current);
}

void MainWindow::on_actionSceneImport_triggered()
{
    // Get filename
    QString filename = QFileDialog::getOpenFileName(this, QString(tr("Import scene")), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "VoukoderPro Scene (*.scene)");
    if (!filename.isEmpty())
    {
        setCursor(Qt::WaitCursor);

        // Load the scene
        std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo = std::make_shared<VoukoderPro::SceneInfo>();
        if (sceneMgr->importScene(sceneInfo, filename.toStdString()) < 0)
        {
            setCursor(Qt::ArrowCursor);

            QMessageBox::critical(this, tr("Error"), tr("An error occured while trying to load the file."), QMessageBox::Button::Ok);

            return;
        }

        // Use the filename as name if empty
        if (sceneInfo->name.empty())
            sceneInfo->name = QFileInfo(filename).fileName().toStdString();

        SceneEditorScene* scene = addSceneTab(sceneInfo);
        scene->markNotModified(false);

        setCursor(Qt::ArrowCursor);
    }
}

void MainWindow::on_actionSceneExport_triggered()
{
    SceneEditorView* view = currentSceneView();
    if (view)
    {
        // Get filename
        const QString filename = QFileDialog::getSaveFileName(this, QString(tr("Export scene")), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "VoukoderPro Scene (*.scene)");
        if (!filename.isEmpty())
        {
            SceneEditorScene* scene = view->editorScene();
            std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo = scene->sceneInfo();

            // Use the filename as name if empty
            if (sceneInfo->name.empty())
                sceneInfo->name = QFileInfo(filename).fileName().toStdString();

            // Save the scene
            if (sceneMgr->exportScene(sceneInfo, filename.toStdString()) < 0)
            {
                QMessageBox::critical(this, tr("Error"), tr("An error occured while trying to export the file."));

                return;
            }
        }
    }
}

void MainWindow::on_actionSceneExit_triggered()
{
    QCoreApplication::exit();
}

// ### NODE ###

void MainWindow::on_actionNodeInputVideo_triggered()
{
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo = std::make_shared<VoukoderPro::NodeInfo>();
    nodeInfo->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    nodeInfo->type = VoukoderPro::NodeInfoType::input;
    nodeInfo->mediaType = VoukoderPro::MediaType::video;
    nodeInfo->data = { { "tracks", nlohmann::ordered_json::array() } };

    createNode(nodeInfo);
}

void MainWindow::on_actionNodeInputAudio_triggered()
{
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo = std::make_shared<VoukoderPro::NodeInfo>();
    nodeInfo->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    nodeInfo->type = VoukoderPro::NodeInfoType::input;
    nodeInfo->mediaType = VoukoderPro::MediaType::audio;
    nodeInfo->data = { { "tracks", nlohmann::ordered_json::array() } };

    createNode(nodeInfo);
}

void MainWindow::on_actionNodeFilterVideo_triggered()
{
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo = std::make_shared<VoukoderPro::NodeInfo>();
    nodeInfo->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    nodeInfo->type = VoukoderPro::NodeInfoType::filter;
    nodeInfo->mediaType = VoukoderPro::MediaType::video;
    nodeInfo->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_VIDEO_FILTER) } };

    createNode(nodeInfo);
}

void MainWindow::on_actionNodeFilterAudio_triggered()
{
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo = std::make_shared<VoukoderPro::NodeInfo>();
    nodeInfo->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    nodeInfo->type = VoukoderPro::NodeInfoType::filter;
    nodeInfo->mediaType = VoukoderPro::MediaType::audio;
    nodeInfo->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_AUDIO_FILTER) } };

    createNode(nodeInfo);
}

void MainWindow::on_actionNodeEncoderVideo_triggered()
{
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo = std::make_shared<VoukoderPro::NodeInfo>();
    nodeInfo->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    nodeInfo->type = VoukoderPro::NodeInfoType::encoder;
    nodeInfo->mediaType = VoukoderPro::MediaType::video;
    nodeInfo->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_VIDEO_ENCODER) } };

    createNode(nodeInfo);
}


void MainWindow::on_actionNodeEncoderAudio_triggered()
{
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo = std::make_shared<VoukoderPro::NodeInfo>();
    nodeInfo->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    nodeInfo->type = VoukoderPro::NodeInfoType::encoder;
    nodeInfo->mediaType = VoukoderPro::MediaType::audio;
    nodeInfo->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_AUDIO_ENCODER) } };

    createNode(nodeInfo);
}


void MainWindow::on_actionNodeMuxer_triggered()
{
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo = std::make_shared<VoukoderPro::NodeInfo>();
    nodeInfo->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    nodeInfo->type = VoukoderPro::NodeInfoType::muxer;
    nodeInfo->mediaType = VoukoderPro::MediaType::mux;
    nodeInfo->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_MUXER) } };

    createNode(nodeInfo);
}

void MainWindow::on_actionNodeOutput_triggered()
{
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo = std::make_shared<VoukoderPro::NodeInfo>();
    nodeInfo->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    nodeInfo->type = VoukoderPro::NodeInfoType::output;
    nodeInfo->mediaType = VoukoderPro::MediaType::mux;
    nodeInfo->data = {
        { "id", prefs.get<std::string>(VPRO_DEFAULT_OUTPUT) },
        { "url", "$(OutputFilename)" }
    };

    createNode(nodeInfo);
}

void MainWindow::on_actionNodePostProc_triggered()
{
    std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo = std::make_shared<VoukoderPro::NodeInfo>();
    nodeInfo->id = QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
    nodeInfo->type = VoukoderPro::NodeInfoType::postproc;
    nodeInfo->mediaType = VoukoderPro::MediaType::out;
    nodeInfo->data = { { "id", prefs.get<std::string>(VPRO_DEFAULT_POSTPROC) } };

    createNode(nodeInfo);
}

void MainWindow::on_actionNodeDelete_triggered()
{
    // Do we have
    SceneEditorView* view = currentSceneView();
    if (!view)
        return;

    SceneEditorScene* scene = view->editorScene();

    // Did we come from the context menu?
    if (view->sourceNode())
        scene->removeNode(*view->sourceNode());
    else
    {
        // Delete all selected nodes if we didn't come from a context menu
        for (const auto node : scene->selectedNodes())
            scene->removeNode(*node);
    }
}

void MainWindow::on_actionNodeProperties_triggered()
{
    // Do we have
    SceneEditorView* view = currentSceneView();
    if (!view)
        return;

    SceneEditorNodeModel* nodeModel = nullptr;

    // Did we come from the context menu?
    if (view->sourceNode())
        nodeModel = static_cast<SceneEditorNodeModel*>(view->sourceNode()->nodeDataModel());
    else
    {
        SceneEditorScene* scene = view->editorScene();
        nodeModel = static_cast<SceneEditorNodeModel*>(scene->selectedNodes().front()->nodeDataModel());
    }

    if (nodeModel && nodeModel->hasProperties())
        nodeModel->showPropertyDialog();
}

void MainWindow::on_menuNode_aboutToShow()
{
    //on_sceneSelectionChanged();
}

void MainWindow::on_menuNode_aboutToHide()
{
    on_sceneSelectionChanged();
}

void MainWindow::createNode(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo)
{
    // Get the current view
    SceneEditorView* view = static_cast<SceneEditorView*>(ui->scenes->currentWidget());
    if (!view)
        return;

    // Set the new nodes position
    nodeInfo->posX = view->nextNodePos().x();
    nodeInfo->posY = view->nextNodePos().y();

    // Reset the next node position
    view->setNextNodePos({ 0, 0 });

    // Create the new node
    SceneEditorScene* scene = view->editorScene();
    QtNodes::Node* targetNode = scene->addNode(nodeInfo);

    // Connect both nodes?
    QtNodes::Node* sourceNode = view->sourceNode();
    if (sourceNode && targetNode)
    {
        const QtNodes::PortIndex sourcePort = 0;
        const QtNodes::PortIndex targetPort = 0;
        const QtNodes::NodeDataModel* sourceModel = sourceNode->nodeDataModel();
        const QtNodes::NodeDataModel* targetModel = targetNode->nodeDataModel();

        // Is it possible to connect both nodes
        if (sourceModel->nPorts(QtNodes::PortType::Out) > 0 && targetModel->nPorts(QtNodes::PortType::In) &&
            sourceModel->dataType(QtNodes::PortType::Out, sourcePort) == targetModel->dataType(QtNodes::PortType::In, targetPort))
        {
            // Finally connect the nodes
            scene->createConnection(*targetNode, targetPort, *sourceNode, sourcePort);
        }
    }
}

// ### PRIVATE ###

QMenu* MainWindow::nodeMenu()
{
    ui->actionNodeProperties->setEnabled(false);
    ui->actionNodeDelete->setEnabled(false);

    return ui->menuNode;
}

QMenu* MainWindow::nodeMenu(QtNodes::Node& node)
{
    ui->actionNodeProperties->setEnabled(true);
    ui->actionNodeDelete->setEnabled(true);

    return ui->menuNode;
}

SceneEditorView* MainWindow::currentSceneView()
{
    QWidget* widget = ui && ui->scenes ? ui->scenes->currentWidget() : nullptr;

    return (widget) ? static_cast<SceneEditorView*>(widget) : nullptr;
}

SceneEditorScene* MainWindow::addSceneTab(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo)
{
    if (!sceneInfo->name.empty())
    {
        // Is that scene already open?
        for (int i = 0; i < ui->scenes->count(); i++)
        {
            SceneEditorView* view = static_cast<SceneEditorView*>(ui->scenes->widget(i));
            SceneEditorScene* scene = view->editorScene();

            if (scene->sceneInfo()->name == sceneInfo->name)
            {
                if(scene->isModified())
                {
                    if (QMessageBox::information(this, tr("Information"),
                                                        tr("A scene with this name is already open.\n\nAre you sure you want to close it and continue opening the scene?"),
                                                        QMessageBox::Button::Ok | QMessageBox::Button::Cancel, QMessageBox::Button::Cancel) == QMessageBox::Button::Ok)
                    {
                        ui->scenes->removeTab(i);
                        break;
                    }
                    else
                    {
                        return scene;
                    }
                }
                else
                {
                    ui->scenes->removeTab(i);
                }
            }
        }
    }

    // Create a new view ...
    SceneEditorScene* scene = new SceneEditorScene(sceneInfo, plugins, ui->scenes);
    scene->markNotModified();

    connect(scene, &SceneEditorScene::changed, this, &MainWindow::markTabModified);

    auto view = new SceneEditorView(scene);
    view->setProperty("name", QString::fromStdString(sceneInfo->name));

    // Add tab
    int idx = ui->scenes->addTab(view, sceneInfo->name.empty() ? getNextUntitledName() : QString::fromStdString(sceneInfo->name));
    ui->scenes->setTabIcon(idx, QIcon(":/fugue/document"));
    ui->scenes->setTabToolTip(idx, sceneInfo->name.empty() ? getNextUntitledName() : QString::fromStdString(sceneInfo->name));
    ui->scenes->setCurrentIndex(idx);

    // Center view on scene
    view->centerScene();

    ui->actionSceneSave->setEnabled(scene->isModified());
    ui->actionSceneSaveAs->setEnabled(true);
    ui->actionSceneClose->setEnabled(true);
    ui->actionSceneExport->setEnabled(true);
    ui->menuNode->setEnabled(true);
    ui->menuNodeAdd->setEnabled(true);
    ui->menuNodeFilter->setEnabled(true);
    ui->menuNodeInput->setEnabled(true);
    ui->menuNodeEncoder->setEnabled(true);
    ui->actionNodeInputVideo->setEnabled(true);
    ui->actionNodeInputAudio->setEnabled(true);
    ui->actionNodeFilterVideo->setEnabled(true);
    ui->actionNodeFilterAudio->setEnabled(true);
    ui->actionNodeEncoderVideo->setEnabled(true);
    ui->actionNodeEncoderAudio->setEnabled(true);
    ui->actionNodeMuxer->setEnabled(true);
    ui->actionNodeOutput->setEnabled(true);
    ui->menuView->setEnabled(true);
    ui->actionViewCenter->setEnabled(true);
    ui->actionToolsTest_scene->setEnabled(true);
    ui->actionToolsPerformance_Test->setEnabled(true);

    connect(scene, &QtNodes::FlowScene::selectionChanged, this, &MainWindow::on_sceneSelectionChanged);

    return scene;
}

bool MainWindow::saveScene(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, bool force)
{
    // Get all available scenes
    std::vector<std::shared_ptr<VoukoderPro::SceneInfo>> sceneInfos;
    vkdrPro->sceneManager()->load(sceneInfos);

    // Does this scene exists already?
    bool exists = std::find_if(sceneInfos.begin(), sceneInfos.end(), [&sceneInfo](std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo2){
                      return sceneInfo->name == sceneInfo2->name;
                  }) != sceneInfos.end();

    if (force || !exists || (exists && QMessageBox::information(this,
                                            tr("Information"),
                                            tr("A scene with this name already exists.\nDo you want to overwrite that scene?"),
                                            QMessageBox::Button::Ok | QMessageBox::Button::Cancel, QMessageBox::Button::Cancel) == QMessageBox::Button::Ok))
    {
        // Save the scene
        if (sceneMgr->save(sceneInfo) < 0)
        {
            QMessageBox::critical(this, tr("Error"), tr("An error occured while trying to save the file."));

            return false;
        }
        else
            return true;
    }

    return false;
}

// ### VIEW ###

void MainWindow::on_actionViewCenter_triggered()
{
    SceneEditorView* view = currentSceneView();
    view->centerScene();
}

// ### MISC ###

QString MainWindow::getNextUntitledName()
{
    QString untitled = tr("Untitled");

    // Find max. number
    int max = -1;
    for (int i = 0; i < ui->scenes->count(); i++)
    {
        const QString name = ui->scenes->tabText(i).toLower().replace(" *", "");
        if (name.startsWith(untitled.toLower()))
        {
            const int number = name.right(name.length() - untitled.length()).toInt();
            if (number > max)
                max = number;
        }
    }

    max++;

    // Append next number
    if (max > 0)
        untitled += QString::number(max);

    return untitled;
}

void MainWindow::on_sceneSelectionChanged()
{
    SceneEditorView* view = currentSceneView();
    if (view)
    {
        SceneEditorScene* scene = view->editorScene();
        const int selectedNodes = scene->selectedNodes().size();

        ui->actionNodeDelete->setEnabled(selectedNodes > 0);
        ui->actionNodeProperties->setEnabled(selectedNodes == 1);
    }
}

void MainWindow::on_actionHelpAbout_triggered()
{
    AboutDialog dialog(vkdrPro, this);
    dialog.exec();
}

void MainWindow::on_actionPreferences_triggered()
{
    PreferencesDialog dialog(this);
    dialog.init(plugins);
    dialog.exec();
}

void MainWindow::markTabModified()
{
    const QString indicator = " *";

    for (int i = 0; i < ui->scenes->count(); i++)
    {
        SceneEditorView* view = static_cast<SceneEditorView*>(ui->scenes->widget(i));
        SceneEditorScene* scene = view->editorScene();
        const bool modified = scene->isModified();

        QString text = ui->scenes->tabText(i);
        if (text.endsWith(indicator) && !modified)
            ui->scenes->setTabText(i, text.left(text.length() - indicator.length()));

        if (!text.endsWith(indicator) && modified)
            ui->scenes->setTabText(i, text + indicator);
    }

    ui->actionSceneSave->setEnabled(currentSceneView()->editorScene()->isModified());
}

void MainWindow::on_scenes_tabCloseRequested(int index)
{
    closeScene(index);
}

void MainWindow::closeScene(int index)
{
    auto closeSceneInternal = [&]()
    {
        QWidget* widget = ui->scenes->widget(index);

        ui->scenes->removeTab(index);

        delete widget;

        if (ui->scenes->count() == 0)
        {
            ui->actionSceneSave->setEnabled(false);
            ui->actionSceneSaveAs->setEnabled(false);
            ui->actionSceneClose->setEnabled(false);
            ui->actionSceneExport->setEnabled(false);
            ui->menuNode->setEnabled(false);
            ui->menuNodeAdd->setEnabled(false);
            ui->menuNodeFilter->setEnabled(false);
            ui->menuNodeInput->setEnabled(false);
            ui->menuNodeEncoder->setEnabled(false);
            ui->actionNodeInputVideo->setEnabled(false);
            ui->actionNodeInputAudio->setEnabled(false);
            ui->actionNodeFilterVideo->setEnabled(false);
            ui->actionNodeFilterAudio->setEnabled(false);
            ui->actionNodeEncoderVideo->setEnabled(false);
            ui->actionNodeEncoderAudio->setEnabled(false);
            ui->actionNodeMuxer->setEnabled(false);
            ui->actionNodeOutput->setEnabled(false);
            ui->menuView->setEnabled(false);
            ui->actionViewCenter->setEnabled(false);
            ui->actionToolsTest_scene->setEnabled(false);
            ui->actionToolsPerformance_Test->setEnabled(false);
        }
    };

    SceneEditorView* view = static_cast<SceneEditorView*>(ui->scenes->widget(index));
    SceneEditorScene* scene = view->editorScene();
    if (scene->isModified())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(QApplication::applicationName());
        msgBox.setText(tr("The scene is modified. If you close it all changes will be lost."));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        if (msgBox.exec() == QMessageBox::Ok)
            closeSceneInternal();
    }
    else
        closeSceneInternal();
}

void MainWindow::on_actionToolsTest_scene_triggered()
{
    SceneEditorView* view = currentSceneView();
    if (view)
    {
        SceneEditorScene* scene = view->editorScene();

        TestRunDialog dialog(vkdrPro, scene->sceneInfo());
        dialog.exec();
    }
}

void MainWindow::on_scenes_currentChanged(int index)
{
    SceneEditorView* view = currentSceneView();
    if (view)
    {
        SceneEditorScene* scene = view->editorScene();
        ui->actionSceneSave->setEnabled(scene->isModified());
    }
}

void MainWindow::on_actionWindowNews_triggered()
{
    onNewsUpdate(false);

    NewsDialog dialog;
    dialog.exec();
}

void MainWindow::on_actionToolsPerformance_Test_triggered()
{
    SceneEditorView* view = currentSceneView();
    if (view)
    {
        SceneEditorScene* scene = view->editorScene();

        PerformanceTestDialog dialog(vkdrPro, scene->sceneInfo());
        dialog.exec();
    }
}

void MainWindow::onNewsUpdate(bool unreadNews)
{
    ui->actionWindowNews->setIcon(QIcon(unreadNews ? ":/fugue/news-new" : ":/fugue/news"));
}
