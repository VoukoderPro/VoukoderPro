#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <boost/dll/import.hpp>

#include "../VoukoderPro/voukoderpro_api.h"
#include "../VoukoderPro/Version.h"

#include "mainwindow.h"
#include "sceneselectdialog.h"

int main(int argc, char *argv[])
{
    // Set up the application
    QApplication a(argc, argv);
    a.setApplicationName(QString(APP_NAME));
    a.setApplicationVersion(APP_VERSION);
    a.setStyle("Fusion");

    // Set up translations
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Designer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    boost::function<pluginapi_create_t> factory;
    std::shared_ptr<VoukoderPro::IClient> vkdrPro = nullptr;

    try
    {
        factory = VoukoderProCreateInstance();
        vkdrPro = factory();
    }
    catch (boost::system::system_error e)
    {
        QMessageBox::critical(nullptr, a.applicationName(), "Unable to load voukoderpro.dll!\n\n" + QString::fromStdString(e.what()));
        return -1;
    }

    int ret = 0;

    // Initialize VoukoderPro
    if ((ret = vkdrPro->init()) < 0)
    {
        QMessageBox::critical(nullptr, a.applicationName(), "VoukoderPro initialization failed!");
        return ret;
    }

    QString selectedScene = "";

    if (argc > 1)
    {
        const QString param = QString::fromLocal8Bit(argv[1]);

        if (param.toLower() == "/sceneselect")
        {
            const QString name = argc > 2 ? QString::fromLocal8Bit(argv[2]) : "";

            SceneSelectDialog dialog(vkdrPro, name);
            if (dialog.exec() == QDialog::Accepted)
            {
                QTextStream out(stdout);
                out << dialog.selectedScene();

                return 0;
            }

            return -1;
        }

        selectedScene = param;
    }

    // Create and show main window
    MainWindow w(vkdrPro, selectedScene);
    w.show();

    return a.exec();
}
