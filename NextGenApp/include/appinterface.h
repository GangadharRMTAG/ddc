#ifndef APPINTERFACE_H
#define APPINTERFACE_H
/**
 * @file appinterface.h
 * @brief Declaration of the AppInterface class.
 *
 * This header defines AppInterface, a QObject-derived class intended to act
 * as a high-level application interface that can expose signals and slots
 * to other parts of the application (for example QML). The class currently
 * provides a constructor and placeholders for signals/slots to be added as
 * the application's needs grow.
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 * @modified - Kunal Kokare
 */

#include <QObject>
#include <QMutex>
#include <QByteArray>
#include <QQueue>
#include <QThread>
#include <QTimer>
#include <zmq.hpp>
#include <QDateTime>
#include<QString>


class AppInterface : public QObject
{
    Q_OBJECT

    /**
     * @property rpm
     * @brief Current engine RPM value for UI.
     *
     * This property represents the scaled RPM value
     * (e.g. 10, 20, 30 corresponding to x1000 RPM).
     * QML updates automatically when rpmChanged() is emitted.
     */
    Q_PROPERTY(int rpm READ rpm NOTIFY rpmChanged)


    /**
     * @property messagePopup
     * @brief .
     *
     * This property represents the scaled MessagePopup value
     * QML updates automatically when popupChanged() is emitted.
     */
    Q_PROPERTY(int popup READ popup NOTIFY popupChanged FINAL)

    /**
     * @property currentTime
     * @brief .
     *
     * This property represents the current time
     * QML updates time automatically with currentTimeChanged for every second.
     */
    Q_PROPERTY(QString currentTime READ currentTime NOTIFY currentTimeChanged)

    /**
     * @property lastResetDate
     * @brief .
     *
     * This property represents the last rest date of trip information
     * QML updates date of trip information on reset button when lastResetDateChanged emmited.
     */
    Q_PROPERTY(QString lastResetDate READ lastResetDate WRITE setLastResetDate NOTIFY lastResetDateChanged)



    /**
     * @property fuelRate
     * @brief Current fuel rate value for UI.
     *
     * This property represents the Average rate of fuel consumption,
     * QML updates automatically when fuelRateChanged() is emitted.
     */
    Q_PROPERTY(float fuelRate READ fuelRate WRITE setFuelRate NOTIFY fuelRateChanged FINAL)

    /**
     * @property fuelUsage
     * @brief Current fuel usage  value for UI.
     *
     * This property represents the Total amount of fuel consumed. its computed based on fuel rate and trip hours,
     * QML updates automatically when fuelUsageChanged() is emitted.
     */
    Q_PROPERTY(float fuelUsage READ fuelUsage WRITE setFuelUsage NOTIFY fuelUsageChanged FINAL)

    /**
     * @property defRate
     * @brief Current defRate  value for UI.
     *
     * This property represents rate at which a vehicle consumes the DEF
     * QML updates automatically when defRateChanged() is emitted.
     */
    Q_PROPERTY(float defRate READ defRate WRITE setDefRate NOTIFY defRateChanged FINAL)

    /**
     * @property lastTripHours
     * @brief last reset value of trip hours for UI.
     *
     * This property represents the exact trip hours when they reset last time on reset button click
     * QML updates automatically when lastTripHoursChanged() is emitted.
     */
    Q_PROPERTY(float lastTripHours READ lastTripHours WRITE setLastTripHours NOTIFY lastTripHoursChanged FINAL)

    /**
     * @property tripHours
     * @brief Calculated since last reset for each trip meter.
     *
     * This property represents the trip hours since last reset for each trip meter
     * QML updates automatically when tripHoursChanged() is emitted.
     */
    Q_PROPERTY(float tripHours READ tripHours WRITE setTripHours NOTIFY tripHoursChanged FINAL)

    /**
     * @property defUsage
     * @brief  Volume of DEF used for the trip since last reset
     *
     * This property represents the Volume of DEF used for the trip since last reset
     * QML updates automatically when defUsageChanged() is emitted.
     */
    Q_PROPERTY(float defUsage READ defUsage WRITE setDefUsage NOTIFY defUsageChanged FINAL)

    /**
     * @property avgEngineLoad
     * @brief  Average load on engine in % terms
     *
     * This property represents the Average load on engine in % terms computed based on trip hours and Current Instantaneous engine load
     * QML updates automatically when avgEngineLoadChanged() is emitted.
     */
    Q_PROPERTY(int avgEngineLoad READ avgEngineLoad WRITE setAvgEngineLoad NOTIFY avgEngineLoadChanged FINAL)
    /**
     * @property telltales
     * @brief Vector of telltale states.
     *
     * Each index corresponds to a telltale defined in the
     * Telltale enum. Value is typically 0 (OFF) or 1 (ON).
     */
    Q_PROPERTY(QVector<int> telltales READ telltales NOTIFY telltalesChanged)

    /**
     * @property gauges
     * @brief Vector representing gauge levels.
     *
     * Each index corresponds to a gauge defined in the
     * GaugeType enum. Values are mapped levels (1–5)
     * derived from percentage-based input data.
     */
    Q_PROPERTY(QVector<int> gauges READ gauges NOTIFY gaugesChanged)

    /**
     * @property engineHours
     * @brief Total engine operating hours.
     *
     * This property represents accumulated engine hours
     * decoded from CAN/ZMQ frames and exposed to QML.
     */
    Q_PROPERTY(float engineHours READ engineHours NOTIFY engineHoursChanged)

    /**
     * @property isoActive
     * @brief Indicates whether ISO safety mode is active.
     *
     * Used by UI to reflect ISO safety button state.
     */
    Q_PROPERTY(bool isoActive READ isoActive NOTIFY isoActiveChanged)

    /**
     * @property creepActive
     * @brief Indicates whether creep mode is active.
     *
     * Used by UI logic to switch RPM widget behavior.
     */
    Q_PROPERTY(bool creepActive READ creepActive NOTIFY creepActiveChanged)

public:
    /**
     * @brief Constructs the AppInterface object.
     *
     * Initializes internal state and starts the ZMQ
     * receive infrastructure by calling initZmq().
     *
     * @param parent Optional QObject parent.
     */
    explicit AppInterface(QObject *parent = nullptr);

    /**
     * @brief Returns the current RPM value.
     *
     * @return RPM value scaled for UI representation.
     */
    int rpm() const { return m_rpm; }

    /**
     * @brief Returns the popup type.
     *
     * @return pop type value as int.
     */

    int popup() const {return m_popup;}

    /**
     * @brief Returns the current fuel rate value.
     *
     * @return fuel rate value as floating point.
     */
    float fuelRate()const{return m_fuelRate;}

    /**
     * @brief Returns the current fuel usage value.
     *
     * @return fuel usage value as floating point.
     */
    float fuelUsage()const {return m_fuelUsage;}

    /**
     * @brief Returns the def rate value.
     *
     * @return def rate value as floating point.
     */
    float defRate()const {return m_defRate;}

    /**
     * @brief Returns the value for last trip hours.
     *
     * @return last trip hourse value as floating point.
     */
    float lastTripHours() const{return m_lastTripHours;}

    /**
     * @brief Returns the value for trip hours.
     *
     * @return trip hourse value as floating point.
     */
    float tripHours() const{return m_tripHours;}

    /**
     * @brief Returns the value for defUsage.
     *
     * @return defUsage value as floating point.
     */
    float defUsage() const{return m_defUsage;}

    /**
     * @brief Returns the value for avgEngineLoad.
     *
     * @return avgEngineLoad value as int.
     */

    int avgEngineLoad() const{return m_avgEngineLoad;}
    /**
     * @brief Returns the current telltale state vector.
     *
     * @return QVector containing telltale ON/OFF states.
     */
    QVector<int> telltales() const { return m_telltales; }

    /**
     * @brief Returns current gauge levels.
     *
     * @return QVector containing mapped gauge levels.
     */
    QVector<int> gauges() const { return m_gauges; }

    /**
     * @brief Returns total engine hours.
     *
     * @return Engine hours value as floating point.
     */
    float engineHours() const { return m_engineHours; }

    /**
     * @brief Publishes button press/release status.
     *
     * Sends button state over ZMQ using predefined
     * CAN/ZMQ button identifiers.
     *
     * @param buttonIndex Index of the button pressed.
     * @param pressed True if pressed, false if released.
     */
    Q_INVOKABLE void publishButtonStatus(int buttonIndex, bool pressed);

    /**
     * @brief Returns ISO safety mode state.
     *
     * @return True if ISO is active.
     */
    bool isoActive() const { return m_isoActive; }

    /**
     * @brief Returns creep mode state.
     *
     * @return True if creep mode is active.
     */
    bool creepActive() const { return m_creepActive; }

    /**
     * @brief Updates creep mode state.
     *
     * Emits creepActiveChanged() if value changes.
     *
     * @param active New creep mode state.
     */
    Q_INVOKABLE void setCreepActive(bool active);

    /**
     * @brief Destructor.
     *
     * Cleans up ZMQ resources and worker thread.
     */
    ~AppInterface();

    /**
     * @enum Telltale
     * @brief Enumeration of supported telltales.
     *
     * Enum values map directly to CAN/ZMQ IDs:
     * 0xDE001000 + enum value
     */
    enum Telltale {
        Stop = 0,
        Caution,
        SeatBelt,
        ParkBrake,
        WorkLamp,
        Beacon,
        Regeneration,
        GridHeater,
        HydraulicLock,
        FootPedal,
        TelltaleCount
    };
    Q_ENUM(Telltale)

    /**
     * @enum SafetyButton
     * @brief Enumeration of supported safety-related buttons.
     *
     * Used for publishing button status events
     * such as ISO, DEF, and Creep controls.
     */
    enum SafetyButton {
        SafetyISO   = 0,
        SaftyDEF    = 1,
        SafetyCreep = 2
    };
    Q_ENUM(SafetyButton)

    /**
     * @enum GaugeType
     * @brief Enumeration of supported gauges.
     *
     * Enum values map directly to CAN/ZMQ IDs:
     * 0xDE004000 + enum value
     */
    enum GaugeType {
        Fuel = 0,
        Coolant,
        Def,
        Battery,
        Hydraulic,
        GaugeCount
    };
    Q_ENUM(GaugeType)

    QString currentTime() const { return m_currentTime; }

    QString lastResetDate() const { return m_lastResetDate; }


signals:
    /**
     * @brief Emitted when RPM value changes.
     *
     * Triggers UI updates for any QML bindings
     * depending on the rpm property.
     */
    void rpmChanged();

    /**
     * @brief Emitted when Pop up type changes.
     *
     * Triggers UI updates for any QML bindings
     * depending on the popup property.
     */
    void popupChanged();
    void currentTimeChanged();

    /**
     * @brief Emitted when fuel rate value changes.
     *
     * Triggers UI updates for any QML bindings
     * depending on the fuelrate property.
     */
    void fuelRateChanged();

    /**
     * @brief Emitted when fuel usage value changes.
     *
     * Triggers UI updates for any QML bindings
     * depending on the fuelusage property.
     */
    void fuelUsageChanged();

    /**
     * @brief Emitted when def rate value changes.
     *
     * Triggers UI updates for any QML bindings
     * depending on the defrate property.
     */
    void defRateChanged();

    /**
     * @brief Emitted when last trip hours changes.
     *
     * Triggers UI updates for any QML bindings
     * depending on the lasttriphours property.
     */
    void lastTripHoursChanged();

    /**
     * @brief Emitted when trip hours changes.
     *
     * Triggers UI updates for any QML bindings
     * depending on the triphours property.
     */
    void tripHoursChanged();

    /**
     * @brief Emitted when def usage value changes.
     *
     * Triggers UI updates for any QML bindings
     * depending on the defusage property.
     */
    void defUsageChanged();

    /**
     * @brief Emitted when avg engine load changes.
     *
     * Triggers UI updates for any QML bindings
     * depending on the avgengineload property.
     */
    void avgEngineLoadChanged();

    /**
     * @brief Emitted when any telltale state changes.
     *
     * Causes QML views bound to the telltales
     * property to refresh.
     */
    void telltalesChanged();

    /**
     * @brief Emitted when any popup type changes.
     *
     * property to refresh.
     */

    void popupTriggred();

    /**
     * @brief Emitted when any gauge level changes.
     *
     * Causes QML components bound to gauges
     * property to refresh their visual state.
     */
    void gaugesChanged();

    /**
     * @brief Emitted when engine hours value changes.
     *
     * Notifies UI to update engine hour display.
     */
    void engineHoursChanged();

    /**
     * @brief Emitted when ISO safety state changes.
     */
    void isoActiveChanged();

    /**
     * @brief Emitted when creep mode state changes.
     */
    void creepActiveChanged();

    void lastResetDateChanged();

private:
    /**
     * @brief Initializes ZMQ communication infrastructure.
     *
     * This function:
     *  - Sets up a worker object in a dedicated QThread
     *  - Creates and configures a ZMQ SUB socket
     *  - Continuously receives frames and enqueues them
     *
     * No UI updates are performed directly here.
     */
    void initZmq();

    /**
     * @brief Entry point for ZMQ subscriber thread.
     *
     * Creates ZMQ context and SUB socket, subscribes
     * to backend publisher and continuously receives
     * CAN-like frames for processing.
     */
    void startZmqSubscriber();

    /**
     * @brief Processes queued frames periodically.
     *
     * Called by a QTimer running in the UI thread.
     * Dequeues one frame at a time and forwards it
     * to processFrame() for decoding.
     */
    void processQueue();

    /**
     * @brief Decodes a single received frame.
     *
     * Identifies the frame based on CAN/ZMQ ID and
     * updates the corresponding application state.
     *
     * @param id Frame identifier.
     * @param payload Raw 8-byte payload.
     */



#ifdef UNIT_TEST
public:
#else
private:
#endif
    void processFrame(uint32_t id, const QByteArray &payload);

private:

    /**
     * @brief Dedicated thread for ZMQ receiving.
     *
     * Keeps blocking socket operations away
     * from the UI thread.
     */
    QThread m_zmqThread;

    /**
     * @brief Timer used to process frame queue.
     *
     * Runs in the main/UI thread to safely
     * update Qt properties and emit signals.
     */
    QTimer* m_queueTimer{nullptr};

    /**
     * @brief Queue holding received frames.
     *
     * Frames are pushed by the ZMQ thread
     * and popped by the UI thread.
     */
    QQueue<QPair<uint32_t, QByteArray>> m_frameQueue;

    /**
     * @brief Mutex protecting frame queue access.
     */
    QMutex m_queueMutex;

    /**
     * @brief Current RPM value exposed to UI.
     */
    int m_rpm = 0;

    /**
     * @brief Current popup value exposed to UI.
     */
    int m_popup = 0;

    /**
     * @brief ZMQ context used for publishing button events.
     */
    zmq::context_t m_pubContext{1};

    /**
     * @brief ZMQ publisher socket for button status.
     */
    zmq::socket_t  m_buttonPublisher{m_pubContext, zmq::socket_type::pub};

    /**
     * @brief Cached ISO safety mode state.
     */
    bool m_isoActive = false;

    /**
     * @brief Cached creep mode state.
     */
    bool m_creepActive = false;

    /**
     * @brief Vector holding telltale ON/OFF states.
     *
     * Indexed using the Telltale enum.
     */
    QVector<int> m_telltales = QVector<int>(TelltaleCount, 1);

    /**
     * @brief Cached raw fuel level value.
     *
     * Stores last received fuel level percentage
     * before mapping it to gauge levels.
     */
    int m_fuelLevel = 0;

    /**
     * @brief Vector holding gauge display levels.
     *
     * Each element stores a mapped gauge level (1–5)
     * corresponding to the GaugeType enum.
     */
    QVector<int> m_gauges = QVector<int>(GaugeCount, 1);

    /**
     * @brief Cached engine hours value.
     *
     * Stores the most recent decoded engine
     * hours received from backend.
     */
    float m_engineHours = 0.0f;

    QString m_currentTime;
    QTimer* m_timeTimer = nullptr;


    /**
     * @brief Current fuel rate value exposed to UI.
     */
    float m_fuelRate=0.0f;

    /**
     * @brief actualFuelRate value for calculating fuel usage.
     */
    float actualFuelRate = 0.0f;

    /**
     * @brief Current fuel usage value exposed to UI.
     */
    float m_fuelUsage= 0.0f;

    /**
     * @brief Current def rate value exposed to UI.
     */
    float m_defRate = 0.0f;

    /**
     * @brief last Trip Hours value exposed to UI.
     */
    float m_lastTripHours= 0.0f;

    /**
     * @brief Trip Hours value exposed to UI.
     */
    float m_tripHours= 0.0f;

    /**
     * @brief def Usage value exposed to UI.
     */
    float m_defUsage= 0.0f;

    /**
     * @brief engine load value exposed to UI.
     */
    float m_avgEngineLoad = 0;

    QString m_lastResetDate;

public slots:

    /**
     * @brief setting fuel rate .
     */
    void setFuelRate(float value);

    /**
     * @brief setting fuel usage .
     */
    void setFuelUsage(float value);

    /**
     * @brief setting def rate .
     */
    void setDefRate(float value);

    /**
     * @brief setting last trip hours.
     */
    void setLastTripHours(float value);

    /**
     * @brief setting trip hours .
     */
    void setTripHours(float value);

    /**
     * @brief setting def usage .
     */
    void setDefUsage(float value);

    /**
     * @brief setting engine load .
     */
    void setAvgEngineLoad(int value);

    /**
     * @brief setting last reset Date for Trip Information.
     */

    void setLastResetDate(const QString &date);

};

#endif // APPINTERFACE_H
