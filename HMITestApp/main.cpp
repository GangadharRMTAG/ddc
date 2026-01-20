/**
 * @file main.cpp
 * @brief Entry point for the ZMQ CAN Test Publisher application.
 * @author  Kunal Kokare
 *
 * This application provides a simple QML-based UI to:
 *  - Publish RPM values
 *  - Toggle telltale states
 *
 * The data is sent over ZMQ and consumed by the main application.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "zmqpublisher.h"
#include "zmqsubscriber.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    /**
     * ZMQ publisher object responsible for sending CAN frames.
     * This object will be accessed directly from QML.
     */
    ZmqPublisher publisher;
    ZmqSubscriber subscriber;
    engine.rootContext()->setContextProperty("zmqPublisher", &publisher);
    engine.rootContext()->setContextProperty("zmqSubscriber", &subscriber);
    const QUrl url(QStringLiteral("qrc:/main.qml"));

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

