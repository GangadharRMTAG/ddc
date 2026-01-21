/**
 * @file main.cpp
 * @brief Application entry point for the NextGenApp Qt/QML application.
 *
 * This file contains the application's main() function which:
 *  - configures Qt application attributes,
 *  - initializes the logging subsystem,
 *  - creates and configures the application interface and QML engine,
 *  - exposes context properties and singletons to QML,
 *  - loads the main QML file and enters the Qt event loop.
 *
 *
 * @note This file relies on cLogger (./include/clogger.h) and AppInterface
 *       (./include/appinterface.h) to be available and initialized.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "./include/clogger.h"
#include "./include/appinterface.h"



/**
 * @brief Application entry point.
 *
 * Initializes Qt application attributes for high DPI scaling (on Qt < 6),
 * creates the QGuiApplication, initializes the cLogger singleton, configures
 * logger levels for the NextGenApp logger, constructs the AppInterface, prepares
 * the QQmlApplicationEngine, exposes context properties and a QML singleton,
 * and then loads the main QML file. If the main QML file fails to load,
 * the application will exit with a non-zero code.
 *
 * Detailed steps performed by main():
 *  1. Set AA_EnableHighDpiScaling attribute for Qt versions < 6.0.0.
 *  2. Construct QGuiApplication instance.
 *  3. Initialize cLogger singleton and set default logging levels for "NextGenApp".
 *     If logger initialization fails, a qCritical message is emitted but the
 *     application continues (logging may be limited).
 *  4. Instantiate AppInterface and QQmlApplicationEngine.
 *  5. Expose the following context properties to QML:
 *     - isPortrait : boolean determined by compile-time ORIENTATION macro.
 *     - appInterface: pointer to the AppInterface instance.
 *  6. Register the Styles QML singleton so it can be used by QML code.
 *  7. Connect QQmlApplicationEngine::objectCreated to handle load failures.
 *  8. Load the main QML resource "qrc:/main.qml".
 *  9. If engine.rootObjects() is empty after load, log an error and return -1.
 * 10. Enter the Qt event loop via app.exec().
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument values.
 * @return int Application exit code (0 on success, non-zero on failure).
 */
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    // Construct the Qt GUI application instance
    QGuiApplication app(argc, argv);

    // Initialize logger first to capture any startup errors
    if (!cLogger::instance().init("NextGenApp_LOG")) {
        // If initialization fails, emit a critical message and continue.
        qCritical() << "Failed to initialize logger. Application may not log properly.";
        // Continue execution but logging may be limited
    } else {
        // Configure logger levels for the "NextGenApp" logger category.
        cLogger::instance().setLoggerLevel(QtDebugMsg,"NextGenApp");
        cLogger::instance().setLoggerLevel(QtWarningMsg,"NextGenApp");
    }

    // Create the application interface that will be exposed to QML
    AppInterface appIf;
    // Create the QML application engine responsible for loading QML UI
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    // Expose useful context properties to QML
    // isPortrait is derived from the compile-time ORIENTATION macro.
    bool isPortrait = QStringLiteral(ORIENTATION) == "PORTRAIT";
    engine.rootContext()->setContextProperty("isPortrait", isPortrait);
    engine.rootContext()->setContextProperty("appInterface", &appIf);

    // Register a QML singleton for styles so it can be imported in QML files.
    qmlRegisterSingletonType(QUrl("qrc:///Singletons/Styles.qml"), "Styles", 1, 0, "Styles");
    qmlRegisterSingletonType(QUrl("qrc:///Singletons/ScreenUtils.qml"), "ScreenUtils", 1, 0, "ScreenUtils");

    // Connect to handle QML load errors: if the engine fails to create the root
    // object for the requested URL, exit the application with error.
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

    // Verify the root object was created successfully
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "QML engine failed to create root objects";
        return -1;
    }

    // Enter the Qt event loop
    return app.exec();
}
