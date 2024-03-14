QT       += core gui uitools network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 shared

include(qtpropertybrowser.pri)

RC_ICONS = voukoderpro.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += _CRT_SECURE_NO_WARNINGS QT_DISABLE_DEPRECATED_UP_TO=0x050F00

SOURCES += \
    aboutdialog.cpp \
    components/SceneEditor/nodes/sceneeditorencodernodemodel.cpp \
    components/SceneEditor/nodes/sceneeditorfilternodemodel.cpp \
    components/SceneEditor/nodes/sceneeditorinputnodemodel.cpp \
    components/SceneEditor/nodes/sceneeditormuxernodemodel.cpp \
    components/SceneEditor/nodes/sceneeditornodemodel.cpp \
    components/SceneEditor/nodes/sceneeditoroutputnodemodel.cpp \
    components/SceneEditor/nodes/sceneeditorpostprocnodemodel.cpp \
    components/SceneEditor/nodes/ui/encoderpropertiesdialog.cpp \
    components/SceneEditor/nodes/ui/filterpropertiesdialog.cpp \
    components/SceneEditor/nodes/ui/inputpropertiesdialog.cpp \
    components/SceneEditor/nodes/ui/muxerpropertiesdialog.cpp \
    components/SceneEditor/nodes/ui/outputpropertiesdialog.cpp \
    components/SceneEditor/nodes/ui/postprocpropertiesdialog.cpp \
    components/SceneEditor/nodes/ui/propertiesdialog.cpp \
    components/SceneEditor/sceneeditorscene.cpp \
    components/SceneEditor/sceneeditorview.cpp \
    components/Test/addeditvideotrackdialog.cpp \
    components/Test/addeditaudiotrackdialog.cpp \
    components/Test/scenetestdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    newsdialog.cpp \
    components/Test/performancetestdialog.cpp \
    preferences.cpp \
    preferencesdialog.cpp \
    sceneopendialog.cpp \
    scenesavedialog.cpp \
    sceneselectdialog.cpp \
    supportdialog.cpp

HEADERS += \
    aboutdialog.h \
    components/SceneEditor/nodes/sceneeditorencodernodemodel.h \
    components/SceneEditor/nodes/sceneeditorfilternodemodel.h \
    components/SceneEditor/nodes/sceneeditorinputnodemodel.h \
    components/SceneEditor/nodes/sceneeditormuxernodemodel.h \
    components/SceneEditor/nodes/sceneeditornodedata.h \
    components/SceneEditor/nodes/sceneeditoroutputnodemodel.h \
    components/SceneEditor/nodes/sceneeditornodemodel.h \
    components/SceneEditor/nodes/sceneeditorpostprocnodemodel.h \
    components/SceneEditor/nodes/ui/encoderpropertiesdialog.h \
    components/SceneEditor/nodes/ui/filterpropertiesdialog.h \
    components/SceneEditor/nodes/ui/inputpropertiesdialog.h \
    components/SceneEditor/nodes/ui/muxerpropertiesdialog.h \
    components/SceneEditor/nodes/ui/outputpropertiesdialog.h \
    components/SceneEditor/nodes/ui/postprocpropertiesdialog.h \
    components/SceneEditor/nodes/ui/propertiesdialog.h \
    components/SceneEditor/sceneeditorscene.h \
    components/SceneEditor/sceneeditorview.h \
    components/Test/addeditvideotrackdialog.h \
    components/Test/addeditaudiotrackdialog.h \
    components/Test/scenetestdialog.h \
    mainwindow.h \
    newsdialog.h \
    components/Test/performancetestdialog.h \
    preferences.h \
    preferencesdialog.h \
    sceneopendialog.h \
    scenesavedialog.h \
    sceneselectdialog.h \
    supportdialog.h

FORMS += \
    aboutdialog.ui \
    components/SceneEditor/nodes/ui/inputpropertiesdialog.ui \
    components/SceneEditor/nodes/ui/propertiesdialog.ui \
    components/Test/addeditvideotrackdialog.ui \
    components/Test/addeditaudiotrackdialog.ui \
    components/Test/scenetestdialog.ui \
    mainwindow.ui \
    newsdialog.ui \
    components/Test/performancetestdialog.ui \
    preferencesdialog.ui \
    sceneopendialog.ui \
    scenesavedialog.ui \
    sceneselectdialog.ui \
    supportdialog.ui

TRANSLATIONS += \
    Designer_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Boost
INCLUDEPATH += $$(BoostHome)
DEPENDPATH += $$(BoostHome)
win32:LIBS += -L$$(BoostHome)\stage\lib
unix:LIBS += -L$$(BoostHome)\stage\lib -llibboost_filesystem-mgw11-mt-x64-1_80

# Node Editor
DEFINES += NODE_EDITOR_STATIC
win32:CONFIG(release, debug|release): LIBS += -L../3rdparty/nodeeditor-v2/lib/ -lnodes
else:win32:CONFIG(debug, debug|release): LIBS += -L../3rdparty/nodeeditor-v2/lib/ -lnodesd
else:unix: LIBS += -L$$PWD/lib/ -lnodes
INCLUDEPATH += ../3rdparty/nodeeditor-v2/include
DEPENDPATH += ../3rdparty/nodeeditor-v2/include

RESOURCES += \
    designer.qrc \
    fugue.qrc

# Sign the target
win32:CONFIG(release, debug|release): QMAKE_POST_LINK += signtool.exe sign /f \"C:\Users\daniel\OneDrive - Voukoder\Business\Zertifikate\daniel_stankewitz_nopass.pfx\" /fd SHA256  /tr http://ts.ssl.com /td sha256 /v $$shell_quote($$OUT_PWD/release/$$TARGET).exe
