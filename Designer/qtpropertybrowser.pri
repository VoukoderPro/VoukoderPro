INCLUDEPATH += C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser
DEPENDPATH += C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
SOURCES += \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtbuttonpropertybrowser.cpp \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qteditorfactory.cpp \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtgroupboxpropertybrowser.cpp \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.cpp \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowserutils.cpp \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtpropertymanager.cpp \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qttreepropertybrowser.cpp \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtvariantproperty.cpp
	
HEADERS += \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtbuttonpropertybrowser.h \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qteditorfactory.h \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtgroupboxpropertybrowser.h \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.h \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowserutils_p.h \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtpropertymanager.h \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qttreepropertybrowser.h \
	C:/Qt/$$QT_VERSION/Src/qttools/src/shared/qtpropertybrowser/qtvariantproperty.h \

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTPROPERTYBROWSER_EXPORT
    else:qtpropertybrowser-uselib:DEFINES += QT_QTPROPERTYBROWSER_IMPORT
}