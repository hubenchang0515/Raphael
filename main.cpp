#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QMessageLogContext>
#include <QQmlContext>
#include <QThread>
#include "clamavthread.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("https://github.com/hubenchang0515/Raphael");

    QQmlApplicationEngine engine;
    ClamAVThread clamAVThread;

    engine.rootContext()->setContextProperty("ClamAV", &clamAVThread);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    return app.exec();
}
