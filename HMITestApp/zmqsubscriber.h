#ifndef ZMQSUBSCRIBER_H
#define ZMQSUBSCRIBER_H

/**
 * @file zmqsubscriber.h
 * @brief Declaration of ZmqSubscriber class.
 *
 * ZmqSubscriber receives CAN-like frames over ZMQ
 * from the Main application and decodes them into
 * high-level UI states.
 *
 * @author Kunal Kokare
 */

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QPair>
#include <QMutex>
#include <QTimer>
#include <QByteArray>
#include <zmq.hpp>

/**
 * @def CAN_ID_BTN_BASE
 * @brief Base CAN/ZMQ identifier for safety button frames.
 */
#define CAN_ID_BTN_BASE  0xDE002000

/**
 * @class ZmqSubscriber
 * @brief Subscribes to ZMQ CAN frames and exposes decoded states to QML.
 *
 * This class runs a ZMQ subscriber in a dedicated thread,
 * receives raw CAN-like frames, queues them safely, and
 * processes them on the Qt event loop.
 *
 * It decodes safety button states (ISO, CREEP) and exposes
 * them as Q_PROPERTY values for QML binding.
 */
class ZmqSubscriber : public QObject
{
    Q_OBJECT

    /**
     * @property isoActive
     * @brief Indicates whether ISO mode is currently active.
     *
     * This property is updated when an ISO button CAN frame
     * is received from the test application.
     */
    Q_PROPERTY(bool isoActive READ isoActive NOTIFY isoActiveChanged)

    /**
     * @property creepActive
     * @brief Indicates whether Creep mode is currently active.
     *
     * This property is updated when a Creep button CAN frame
     * is received from the test application.
     */
    Q_PROPERTY(bool creepActive READ creepActive NOTIFY creepActiveChanged)

public:
    /**
     * @brief Constructs the ZMQ subscriber and starts receiver thread.
     * @param parent Optional QObject parent.
     */
    explicit ZmqSubscriber(QObject *parent = nullptr);

    /**
     * @brief Stops the ZMQ subscriber thread and cleans up resources.
     */
    ~ZmqSubscriber();

    /**
     * @brief Returns current ISO active state.
     */
    bool isoActive() const { return m_isoActive; }

    /**
     * @brief Returns current Creep active state.
     */
    bool creepActive() const { return m_creepActive; }

signals:
    /**
     * @brief Emitted when a raw CAN-like frame is received.
     *
     * Useful for debugging or logging purposes.
     *
     * @param id CAN identifier
     * @param payload 8-byte CAN payload
     */
    void frameReceived(uint32_t id, QByteArray payload);

    /**
     * @brief Emitted when ISO button state changes.
     *
     * @param pressed true if ISO is active
     */
    void isoButtonChanged(bool pressed);

    /**
     * @brief Emitted when Creep button state changes.
     *
     * @param pressed true if Creep is active
     */
    void creepButtonChanged(bool pressed);

    /**
     * @brief Notifies QML that isoActive property has changed.
     */
    void isoActiveChanged();

    /**
     * @brief Notifies QML that creepActive property has changed.
     */
    void creepActiveChanged();

private slots:
    /**
     * @brief Entry point for ZMQ subscriber thread.
     *
     * Creates and runs a blocking ZMQ SUB socket loop.
     */
    void startZmqSubscriber();

    /**
     * @brief Processes queued CAN frames on Qt event loop.
     *
     * Ensures thread-safe frame handling.
     */
    void processQueue();

private:
    /**
     * @brief Decodes a single CAN frame.
     *
     * @param id CAN identifier
     * @param payload 8-byte CAN payload
     */
    void processFrame(uint32_t id, const QByteArray &payload);

    /**
     * @brief Cached ISO active state.
     */
    bool m_isoActive {false};

    /**
     * @brief Cached Creep active state.
     */
    bool m_creepActive {false};

    /**
     * @brief Dedicated thread for blocking ZMQ receive loop.
     */
    QThread m_zmqThread;

    /**
     * @brief Timer to process queued frames periodically.
     */
    QTimer *m_queueTimer {nullptr};

    /**
     * @brief Thread-safe queue holding received frames.
     */
    QQueue<QPair<uint32_t, QByteArray>> m_frameQueue;

    /**
     * @brief Mutex protecting the frame queue.
     */
    QMutex m_queueMutex;
};

#endif // ZMQSUBSCRIBER_H
