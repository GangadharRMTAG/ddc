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

    // Initialize logger first to capture any startup errors
    if (!cLogger::instance().init("DDC_LOG")) {
        qCritical() << "Failed to initialize logger. Application may not log properly.";
        // Continue execution but logging may be limited
    } else {
        cLogger::instance().setLoggerLevel(QtDebugMsg,"DDC");
        cLogger::instance().setLoggerLevel(QtWarningMsg,"DDC");
    }

    AppInterface appIf;
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    
    // Set context properties
    bool isPortrait = QStringLiteral(ORIENTATION) == "PORTRAIT";
    engine.rootContext()->setContextProperty("isPortrait", isPortrait);
    engine.rootContext()->setContextProperty("appInterface", &appIf);
    
    // Register singleton types
    qmlRegisterSingletonType(QUrl("qrc:///Singletons/Styles.qml"), "Styles", 1, 0, "Styles");

    // Connect to handle QML load errors
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                qCritical() << "Failed to load QML file:" << objUrl.toString();
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);
    
    // Load the main QML file
    engine.load(url);
    
    // Verify the root object was created
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "QML engine failed to create root objects";
        return -1;
    }

    return app.exec();
}
