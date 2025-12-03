#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "./include/clogger.h"
#include "./include/appinterface.h"



int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    AppInterface appIf;
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.rootContext()->setContextProperty("isPortrait", QStringLiteral(ORIENTATION) == "PORTRAIT" ? true : false);
    engine.rootContext()->setContextProperty("appInterface", &appIf);
    qmlRegisterSingletonType(QUrl("qrc:///Singletons/Styles.qml"), "Styles", 1, 0, "Styles");

    cLogger::instance().init("DDC_LOG");
    cLogger::instance().setLoggerLevel(QtDebugMsg,"DDC");
    cLogger::instance().setLoggerLevel(QtWarningMsg,"DDC");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
