#include <QtGui/QGuiApplication>
#include <QQmlEngine>
#include <QDebug>
#include <QScreen>

#include "qtquick2applicationviewer.h"

#include "src/core/Application.h"

#include <plxFramework/utils/log.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    new CLog(LOG_INFO, qApp->applicationDirPath() +"/application.log");
    CLog::log(LOG_INFO, "PLEXUS", "Hello World");
    qDebug() << qApp->libraryPaths();

    CApplication* application = new CApplication();

    QQmlContext* context = application->rootContext();
    context->setContextProperty("screenGeometry", application->screen()->availableGeometry());
    context->setContextProperty("application", application);

//    application->setSource(QUrl("..Resources/qml/main.qml"));
    application->setSource(QUrl("qrc:/qml/main.qml"));

//    application->setFlags(Qt::WindowFullscreenButtonHint);
    application->connect(application->engine(), SIGNAL(quit()), SLOT(close()));
//    |application->showFullScreen();
    application->showMaximized();

    return app.exec();
}
