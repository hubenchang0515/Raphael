#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QMessageLogContext>
#include <clamav/clamav.h>

int main(int argc, char *argv[])
{
    cl_init(CL_INIT_DEFAULT);
    struct cl_engine* clEngine = cl_engine_new();
    if(clEngine == nullptr)
    {
        return 1;
    }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec() && cl_engine_free(clEngine);
}
