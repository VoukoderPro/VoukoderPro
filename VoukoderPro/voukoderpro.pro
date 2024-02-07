QT += core
TEMPLATE = lib
CONFIG += c++17
TARGET = VoukoderPro

DEFINES += _CRT_SECURE_NO_WARNINGS

SOURCES += \
	EncoderNode.cpp \
	FilterNode.cpp \
	InputNode.cpp \
	MuxerNode.cpp  \
	OutputNode.cpp \
	PluginManager.cpp \
	PostProcNode.cpp \
	Router.cpp \
	SceneManager.cpp \
	voukoderpro.cpp \

HEADERS += \
	BaseNode.h \
	EncoderNode.h \
	ffmpeg.h \
	FilterNode.h \
	InputNode.h \
	json-root-schema.h \
	json-schema.hpp \
	json.hpp \
	Logger.h \
	MuxerNode.h \
	OutputNode.h \
	PluginManager.h \
	PostProcNode.h \
	Router.h \
	SceneManager.h \
	System.h \
	types.h \
	voukoderpro.h \
	VoukoderPro_API.h

LIBS += -lAdvapi32 -lOle32 -lOleAut32

# Boost
INCLUDEPATH += $$(BoostHome)
DEPENDPATH += $$(BoostHome)
win32:LIBS += -L$$(BoostHome)\stage\lib
unix:LIBS += -L$$(BoostHome)\stage\lib -llibboost_filesystem-mgw11-mt-x64-1_80

# FFmpeg
win32:CONFIG(release, debug|release) {
  INCLUDEPATH += ../3rdparty/$$(FFmpegDir)-release/include
  DEPENDPATH += ../3rdparty/$$(FFmpegDir)-release/include
  LIBS += -L../3rdparty/$$(FFmpegDir)-release/lib/ -lavcodec-voukoderpro -lavdevice-voukoderpro -lavfilter-voukoderpro -lavformat-voukoderpro -lavutil-voukoderpro -lswresample-voukoderpro -lswscale-voukoderpro
} else:win32:CONFIG(debug, debug|release) {
  INCLUDEPATH += ../3rdparty/$$(FFmpegDir)-debug/include
  DEPENDPATH += ../3rdparty/$$(FFmpegDir)-debug/include
  LIBS += -L../3rdparty/$$(FFmpegDir)-debug/lib/ -lavcodec-voukoderpro -lavdevice-voukoderpro -lavfilter-voukoderpro -lavformat-voukoderpro -lavutil-voukoderpro -lswresample-voukoderpro -lswscale-voukoderpro
}

# Infoware
INCLUDEPATH += ../3rdparty/infoware/include
DEPENDPATH += ../3rdparty/infoware/include
win32:CONFIG(release, debug|release): LIBS += -L../3rdparty/infoware/lib/ -linfoware
else:win32:CONFIG(debug, debug|release): LIBS += -L../3rdparty/infoware/lib/ -linfowared

# Deploy to work dir
win32:win32:CONFIG(debug, debug|release) {
  QMAKE_POST_LINK = $$PWD/post_build_win32.bat
}
