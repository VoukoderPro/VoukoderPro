#pragma once

#include <QMainWindow>
#include <boost/function.hpp>

#include "../VoukoderPro/voukoderpro_api.h"
#include <nodes/DataModelRegistry>
#include <nodes/internal/Node.hpp>
#include "components/SceneEditor/sceneeditorview.h"
#include "preferences.h"
#include "newsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

Q_DECLARE_METATYPE(VoukoderPro::NodeInfo)
Q_DECLARE_METATYPE(VoukoderPro::SceneInfo)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<VoukoderPro::IClient> vkdrPro, const QString id = "", QWidget *parent = nullptr);
    ~MainWindow();
    QMenu* nodeMenu();
    QMenu* nodeMenu(QtNodes::Node& node);

public slots:
    void on_sceneSelectionChanged();

private slots:
    void on_actionSceneNewEmpty_triggered();
    void on_actionSceneNewSimple_triggered();
    void on_actionSceneOpen_triggered();
    void on_actionSceneSave_triggered();
    void on_actionSceneSaveAs_triggered();
    void on_actionSceneClose_triggered();
    void on_actionSceneImport_triggered();
    void on_actionSceneExport_triggered();
    void on_actionSceneExit_triggered();
    void on_actionNodeInputVideo_triggered();
    void on_actionNodeInputAudio_triggered();
    void on_actionNodeFilterVideo_triggered();
    void on_actionNodeFilterAudio_triggered();
    void on_actionNodeEncoderVideo_triggered();
    void on_actionNodeEncoderAudio_triggered();
    void on_actionNodeMuxer_triggered();
    void on_actionNodeOutput_triggered();
    void on_actionNodeDelete_triggered();
    void on_actionNodeProperties_triggered();
    void on_actionViewCenter_triggered();
    void on_menuNode_aboutToShow();
    void on_menuNode_aboutToHide();
    void on_actionHelpAbout_triggered();
    void on_scenes_tabCloseRequested(int index);
    void on_actionToolsTest_scene_triggered();
    void on_actionPreferences_triggered();
    void on_scenes_currentChanged(int index);
    void on_actionNodePostProc_triggered();
    void on_actionSceneNewFilter_triggered();
    void on_actionWindowNews_triggered();
    void on_actionToolsPerformance_Test_triggered();

private slots:
    void onNewsUpdate(bool news);

private:
    void closeScene(int index);
    SceneEditorScene* addSceneTab(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo);
    SceneEditorView* currentSceneView();
    QString getNextUntitledName();
    void createNode(std::shared_ptr<VoukoderPro::NodeInfo> nodeInfo);
    void markTabModified();
    bool saveScene(std::shared_ptr<VoukoderPro::SceneInfo> sceneInfo, bool force = false);

private:
    Ui::MainWindow *ui;
    std::shared_ptr<VoukoderPro::IClient> vkdrPro;
    std::vector<VoukoderPro::AssetInfo> plugins;
    std::shared_ptr<VoukoderPro::ISceneManager> sceneMgr;
    NewsDialog* newsDialog;
    Preferences& prefs = Preferences::instance();
};
