/**
 * @file src/appinterface.cpp
 * @brief Implementation of the AppInterface class.
 *
 * This file contains the implementation details for the AppInterface
 * class which acts as the application-facing interface (for example,
 * to QML or other consumers).
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 * @modified - Kunal Kokare , Sailee Shuddhalwar
 */


#include "../include/appinterface.h"
#include <QTimer>
#include <QObject>
#include <zmq.hpp>
#include <QByteArray>
#include <QDebug>
#include "../include/constants.h"


/**
 * @brief Constructs an AppInterface instance.
 *
 * Creates a new AppInterface object and forwards the optional
 * parent pointer to QObject. AppInterface serves as an application
 * boundary object where application-level slots, signals and methods
 * can be exposed.
 *
 * @param parent Optional parent QObject; defaults to nullptr.
 */


/**
 * @brief Maps a percentage value to a gauge level (1-8).
 *
 * Converts a percentage value (0-100) into 8 discrete gauge levels.
 *
 * Mapping:
 *  - 0–12%   -> Level 1
 *  - 13–25%  -> Level 2
 *  - 26–38%  -> Level 3
 *  - 39–50%  -> Level 4
 *  - 51–63%  -> Level 5
 *  - 64–75%  -> Level 6
 *  - 76–88%  -> Level 7
 *  - 89–100% -> Level 8
 *
 * @param percent Percentage value (0–100).
 * @return Gauge level (1–8).
 */
static int mapPercent(int percent)
{
    if (percent <= LEVEL1) return 1;
    if (percent <= LEVEL2)  return 2;
    if (percent <= LEVEL3)  return 3;
    if (percent <= LEVEL4)  return 4;
    if (percent <= LEVEL5)  return 5;
    if (percent <= LEVEL6)  return 6;
    if (percent <= LEVEL7)  return 7;
    return 8; //LEVEL8
}

static float percentToLiters(int percent)
{
    percent = qBound(0, percent, 100);
    return (percent / 100.0f) * FUEL_TANK_CAPACITY_L;
}
/**
 * @brief Constructs an AppInterface instance.
 *
 * Initializes the ZMQ publisher socket for button events and starts
 * the ZMQ subscriber infrastructure. The publisher binds to port 5556
 * for sending button status updates to the backend system.
 *
 * @param parent Optional QObject parent for memory management.
 *
 * @note The ZMQ subscriber thread is started automatically via initZmq().
 * @note Publisher socket binds to "tcp://*:5556" for button events.
 */
AppInterface::AppInterface(QObject *parent)
    : QObject(parent)
{
    #ifndef UNIT_TEST
        m_buttonPublisher.bind("tcp://*:5556");
        initZmq();
    #else
        // Skip ZMQ socket setup in unit tests
    #endif
    setLastResetDate(LAST_RESET_DATE);
    setLastTripHours(0.0f);
    // ================= TIME SERVICE =================
    m_timeTimer = new QTimer(this);

    connect(m_timeTimer, &QTimer::timeout, this, [this]() {
        QString newTime = QDateTime::currentDateTime().toString("hh:mm AP");

        if (newTime != m_currentTime) {
            m_currentTime = newTime;
            emit currentTimeChanged();
        }
    });

    m_timeTimer->start(1000);

    // Set immediately (no 1s delay)
    m_currentTime = QDateTime::currentDateTime().toString("hh:mm AP");
    emit currentTimeChanged();
}

/**
 * @brief Initializes ZMQ communication infrastructure.
 *
 * Sets up the ZMQ subscriber thread and frame processing timer.
 * The subscriber thread will handle blocking ZMQ operations,
 * while the timer processes queued frames in the UI thread.
 *
 * @note Timer interval is set to 5ms for responsive frame processing.
 * @note ZMQ thread uses DirectConnection for signal/slot communication.
 * @note Timer is parented to this object for automatic cleanup.
 */
void AppInterface::initZmq()
{

    connect(&m_zmqThread, &QThread::started,
            this, &AppInterface::startZmqSubscriber,
            Qt::DirectConnection);


    m_queueTimer = new QTimer(this);
    connect(m_queueTimer, &QTimer::timeout,
            this, &AppInterface::processQueue);
    m_queueTimer->start(5);

    m_zmqThread.start();
}



/**
 * @brief Entry point for ZMQ subscriber thread.
 *
 * Creates a ZMQ context and SUB socket, connects to the backend
 * publisher, and continuously receives CAN-like frames. Each received
 * frame is validated and enqueued for processing by the UI thread.
 *
 * This function runs in a dedicated thread to prevent blocking the UI.
 * It continues until the thread receives an interruption request.
 *
 * @note Connects to LOCAL_HOST_IP (localhost).
 * @note Subscribes to all messages (empty filter).
 * @note Minimum message size is 12 bytes (4-byte ID + 8-byte payload).
 * @note Thread-safe enqueueing using m_queueMutex.
 */
void AppInterface::startZmqSubscriber()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);


    try {
        subscriber.connect(LOCAL_HOST_IP); //localhost IP
        subscriber.set(zmq::sockopt::subscribe, "");
    } catch (const zmq::error_t& e) {
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




/**
 * @brief Processes queued frames periodically.
 *
 * Called by the QTimer every 5ms. Dequeues one frame from the
 * thread-safe queue and forwards it to processFrame() for decoding.
 * This ensures frame processing happens in the UI thread, allowing
 * safe property updates and signal emissions.
 *
 * @note This method runs in the main/UI thread.
 * @note Uses QMutexLocker for thread-safe queue access.
 * @note Returns immediately if queue is empty.
 */
void AppInterface::processQueue()
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

/**
 * @brief Decodes a single received CAN/ZMQ frame.
 *
 * Identifies the frame type based on the frame ID and updates the
 * corresponding application state. Supported frame types include:
 * - RPM frames (CAN_ID_RPM)
 * - Telltale frames (CAN_ID_TELLTALES-0xDE001009)
 * - Gauge frames (CAN_ID_FUEL_LEVEL-0xDE004004)
 * - Engine hours (CAN_ID_ENGINEHOURS)
 * - Safety button frames (CAN_ID_BTN_BASE-0xDE002007)
 *
 * @param id Frame identifier (CAN/ZMQ ID).
 * @param payload Raw 8-byte CAN payload data.
 *
 * @note Only emits signals if the state value actually changes.
 * @note Validates payload size before processing.
 * @note Frame IDs are defined as constants in appinterface.h.
 */
void AppInterface::processFrame(uint32_t id, const QByteArray &payload)
{
    if (payload.size() < 8)
        return;

    if (payload.isNull() || payload.isEmpty()) {
        qWarning("Null or empty payload");
        return;
    }

    const uchar* buf =
        reinterpret_cast<const uchar*>(payload.constData());

    // RPM
    if (id == CAN_ID_RPM) {// c
        int rawRpm = (buf[6] << 8) | buf[7];
        int uiRpm = rawRpm;

        if (uiRpm != m_rpm) {
            m_rpm = uiRpm;
            emit rpmChanged();
        }
        return;
    }

    // ENUM-based telltales
    if (id >= CAN_ID_TELLTALES && id < CAN_ID_TELLTALES + TelltaleCount) {
        int index = id - CAN_ID_TELLTALES;

        if (index < 0 || index >= TelltaleCount) {
            qWarning("Invalid telltale index") ;
            return;
        }

        int value = buf[7] & 0x01;

        if (m_telltales[index] != value) {
            m_telltales[index] = value;
            emit telltalesChanged();
        }
    }

    if (id == CAN_ID_POPUP) {
        int rawPopup = (buf[6] << 8) | buf[7];

        if (rawPopup != m_popup) {
            m_popup = rawPopup;
            qDebug("popTriggred recieved");
            emit popupTriggred();
            emit popupChanged();
            qDebug()<<"value: "<< m_popup;

        }
        return;
    }

    //  GAUGES
    if (id >= CAN_ID_FUEL_LEVEL && id < CAN_ID_FUEL_LEVEL + GaugeCount) {

        int gaugeIndex = id - CAN_ID_FUEL_LEVEL;
        int percent = buf[7];
        int level = 3;

        switch (gaugeIndex) {
        case Fuel:
            level = mapPercent(percent);
            break;

        case Coolant:
            level = mapPercent(percent);
            break;

        case Def:
            level = mapPercent(percent);
            break;

        case Battery:
            level = mapPercent(percent);
            break;

        case Hydraulic:
            level = mapPercent(percent);
            break;

        default:
            return;
        }

        if (m_gauges[gaugeIndex] != level) {
            m_gauges[gaugeIndex] = level;
            emit gaugesChanged();
        }
        return;
    }
    //  ENGINE HOURS
    if (id == CAN_ID_ENGINEHOURS) {

        uint32_t raw =
            (buf[4] << 24) |
            (buf[5] << 16) |
            (buf[6] << 8)  |
            buf[7];

        float hours = raw / 10.0f;

        if (!qFuzzyCompare(m_engineHours, hours)) {
            m_engineHours = hours;
            emit engineHoursChanged();

            // Recalculate trip hours
            m_tripHours = m_engineHours - m_lastTripHours;
            if (m_tripHours < 0) m_tripHours = 0;

            emit tripHoursChanged();
        }
        return;
    }

    // Safty Buttons
    if (id >= CAN_ID_BTN_BASE && id < CAN_ID_BTN_BASE + 8) {

        int index = id - CAN_ID_BTN_BASE;
        bool pressed = buf[7] & 0x01;

        qDebug()<<"[MAIN] Safety Button Index:"<<index <<"State:" << pressed;

        switch (index) {

        case SafetyISO:
            if (m_isoActive != pressed) {
                m_isoActive = pressed;
                emit isoActiveChanged();
            }
            break;

        case SafetyCreep:
            if (m_creepActive != pressed) {
                m_creepActive = pressed;
                emit creepActiveChanged();
            }
            break;

        default:
            qWarning()<<"[MAIN] Unknown Safety Button index:" <<index;
            break;
        }

        return;
    }

    if (id == CAN_ID_FUELRATE) {
        int fuelRate = (buf[6] << 8) | buf[7];


        if (fuelRate != m_fuelRate) {
            m_fuelRate = fuelRate;
            actualFuelRate = m_fuelRate / 20;
            m_fuelUsage += actualFuelRate * (60 / 3600.0);
            emit fuelRateChanged();
            emit fuelUsageChanged();
        }

        return;
    }

    if (id == CAN_ID_DEFRATE) {
        int defRate = (buf[6] << 8) | buf[7];


        if (defRate != m_defRate) {
            m_defRate = defRate;
            m_defUsage = m_defRate * m_tripHours;

            emit defRateChanged();
            emit defUsageChanged();
        }
        return;
    }


    if (id == CAN_ID_ENGINELOAD) {
        int engineLoad = (buf[6] << 8) | buf[7];

        if (engineLoad != m_avgEngineLoad) {
            m_avgEngineLoad = engineLoad ;
            emit avgEngineLoadChanged();
        }
        return;
    }

}

/**
 * @brief Publishes button press/release status over ZMQ.
 *
 * Constructs a CAN-like frame with the button identifier and state,
 * then publishes it via the ZMQ publisher socket. The frame format
 * follows the standard CAN frame structure with 4-byte ID and
 * 8-byte payload.
 *
 * @param buttonIndex Button identifier (0-7, maps to SafetyButton enum).
 * @param pressed True if button is pressed, false if released.
 *
 * @note Frame ID is calculated as: CAN_ID_BTN_BASE + buttonIndex
 * @note Button state is encoded in payload byte 7, bit 0
 */
void AppInterface::publishButtonStatus(int buttonIndex, bool pressed)
{
    uint32_t id = CAN_ID_BTN_BASE + buttonIndex;

    QByteArray payload(8, 0x00);
    payload[7] = pressed ? 1 : 0;

    QByteArray frame;
    frame.append(reinterpret_cast<char*>(&id), sizeof(id));
    frame.append(payload);

    zmq::message_t msg(frame.size());
    memcpy(msg.data(), frame.data(), frame.size());

    m_buttonPublisher.send(msg, zmq::send_flags::none);

    qDebug()<<"[MAIN] Published Button Status. Index:"<< buttonIndex << "State:" << pressed;
}

void AppInterface:: setFuelRate(float val){

    m_fuelRate = val;
    emit fuelRateChanged();

}

void AppInterface:: setFuelUsage(float val){
    if(val < 0.0f) {
        m_fuelUsage = 0.0f;}
    else if (val > FUEL_USAGE) {
        m_fuelUsage = FUEL_USAGE;
    } else {
        m_fuelUsage = val;
    }

    emit fuelUsageChanged();
}

/**
 * @brief Sets the creep mode active state.
 *
 * Updates the internal creep mode state and emits creepActiveChanged()
 * signal if the value actually changed. This method is invokable from QML.
 *
 * @param active New creep mode state (true = active, false = inactive).
 *
 * @note This method is Q_INVOKABLE and can be called from QML.
 * @note Signal is only emitted if the state actually changes.
 */

void AppInterface::setCreepActive(bool active)
{
    if (m_creepActive == active)
        return;

    m_creepActive = active;
    emit creepActiveChanged();
}


/**
 * @brief Destructor for AppInterface.
 *
 * Performs graceful shutdown of ZMQ resources:
 * 1. Requests thread interruption
 * 2. Waits up to 5 seconds for thread to exit
 * 3. Force terminates if thread is still running
 *
 * Also closes ZMQ sockets and cleans up resources.
 * The QTimer and other Qt objects are automatically cleaned up
 * via Qt's parent-child ownership mechanism.
 *
 * @note Thread cleanup is critical to prevent resource leaks.
 * @note Force termination is used as last resort if graceful shutdown fails.
 */
void AppInterface::setDefRate(float val)
{
    if (qFuzzyCompare(m_defRate, val))
        return;

    m_defRate = val;
    emit defRateChanged();
}

void AppInterface::setTripHours(float)
{
    // Trip hours is derived value
    m_tripHours = m_engineHours - m_lastTripHours;
    if (m_tripHours < 0) m_tripHours = 0;

    emit tripHoursChanged();
}


void AppInterface::setLastTripHours(float value)
{
    // value = engine hours at reset moment
    m_lastTripHours = value;
    emit lastTripHoursChanged();

    // Recalculate trip
    m_tripHours = m_engineHours - m_lastTripHours;
    if (m_tripHours < 0) m_tripHours = 0;

    emit tripHoursChanged();
}

void AppInterface :: setDefUsage(float value){

    m_defUsage = value;
    qDebug()<<"m_defUsage"<<m_defUsage;

    emit defUsageChanged();
}

void AppInterface :: setAvgEngineLoad(int value){

    m_avgEngineLoad = value;
    emit avgEngineLoadChanged();
}

void AppInterface::setLastResetDate(const QString &date)
{
    if (m_lastResetDate == date)
        return;

    m_lastResetDate = date;
    emit lastResetDateChanged();
}


AppInterface::~AppInterface() {
    // Request thread interruption
    m_zmqThread.requestInterruption();
    m_zmqThread.quit();
    m_zmqThread.wait(5000); // Wait up to 5 seconds
    if (m_zmqThread.isRunning()) {
        qWarning("ZMQ thread did not stop gracefully !!");
        m_zmqThread.terminate();
    }
}



