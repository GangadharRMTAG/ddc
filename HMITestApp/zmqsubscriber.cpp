#include "zmqsubscriber.h"
#include <QtLogging>
#include <QDebug>

#define CAN_ID_BTN_ISO  0xDE002000

ZmqSubscriber::ZmqSubscriber(QObject *parent)
    : QObject(parent)
{
    connect(&m_zmqThread, &QThread::started,
            this, &ZmqSubscriber::startZmqSubscriber,
            Qt::DirectConnection);

    m_queueTimer = new QTimer(this);
    connect(m_queueTimer, &QTimer::timeout,
            this, &ZmqSubscriber::processQueue);
    m_queueTimer->start(5);

    m_zmqThread.start();
}

void ZmqSubscriber::startZmqSubscriber()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);

    try {
        subscriber.connect("tcp://127.0.0.1:5556");   // Main → Test
        subscriber.set(zmq::sockopt::subscribe, "");
        qInfo("ZmqSubscriber connected to tcp://127.0.0.1:5556");
    } catch (const zmq::error_t &e) {
        qCritical("ZMQ connection failed: %s", e.what());
        return;
    }

    while (!QThread::currentThread()->isInterruptionRequested()) {

        zmq::message_t msg;
        if (!subscriber.recv(msg, zmq::recv_flags::none))
            continue;

        if (msg.size() < 12) {
            qWarning("Received ZMQ message too small: %zu bytes", msg.size());
            continue;
        }

        uint32_t id;
        memcpy(&id, msg.data(), 4);

        QByteArray payload(
            static_cast<char*>(msg.data()) + 4, 8);

        {
            QMutexLocker locker(&m_queueMutex);
            m_frameQueue.enqueue({id, payload});
        }
    }
}

void ZmqSubscriber::processQueue()
{
    QPair<uint32_t, QByteArray> frame;

    {
        QMutexLocker locker(&m_queueMutex);
        if (m_frameQueue.isEmpty())
            return;

        frame = m_frameQueue.dequeue();
    }

    processFrame(frame.first, frame.second);
}



void ZmqSubscriber::processFrame(uint32_t id, const QByteArray &payload)
{
    if (payload.size() < 8)
        return;

    const uchar *buf =
        reinterpret_cast<const uchar *>(payload.constData());

    emit frameReceived(id, payload);

    if (id >= CAN_ID_BTN_BASE && id < CAN_ID_BTN_BASE + 8) {

        int index = id - CAN_ID_BTN_BASE;
        bool pressed = buf[7] & 0x01;

        if (index == 0) { // ISO
            if (m_isoActive != pressed) {
                m_isoActive = pressed;
                emit isoActiveChanged();
            }
            emit isoButtonChanged(pressed);
        }

        else if (index == 2) { // ✅ CREEP
            if (m_creepActive != pressed) {
                m_creepActive = pressed;
                emit creepActiveChanged();
                qDebug()<<"[TEST] Creep state updated ="<<pressed;
            }
        }
    }
}

ZmqSubscriber::~ZmqSubscriber()
{
    m_zmqThread.requestInterruption();
    m_zmqThread.quit();
    m_zmqThread.wait(5000);

    if (m_zmqThread.isRunning()) {
        qWarning("ZmqSubscriber thread did not stop gracefully");
        m_zmqThread.terminate();
    }
}
