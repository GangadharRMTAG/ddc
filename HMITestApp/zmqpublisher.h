#ifndef ZMQPUBLISHER_H
#define ZMQPUBLISHER_H

/**
 * @file zmqpublisher.h
 * @brief Declaration of ZmqPublisher class.
 *
 * ZmqPublisher sends CAN-like frames over ZMQ
 * for testing and simulation purposes.
 *
 * @author Kunal Kokare
 */

#include <QObject>
#include <zmq.hpp>

/**
 * @def CAN_ID_FUEL_LEVEL
 * @brief Base CAN/ZMQ identifier for gauge-related data.
 */
#define CAN_ID_FUEL_LEVEL   0xDE004000

/**
 * @class ZmqPublisher
 * @brief Publishes RPM and telltale CAN frames via ZMQ.
 *
 * Used by the test application to simulate CAN data
 * for the main UI application.
 */
class ZmqPublisher : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs and binds ZMQ publisher socket.
     *
     * Initializes ZMQ context and PUB socket.
     *
     * @param parent Optional QObject parent.
     */
    explicit ZmqPublisher(QObject *parent = nullptr);
   Q_INVOKABLE void messagePopup(int);
   Q_INVOKABLE float currentEngineHours() const { return m_engineHours; }
   Q_INVOKABLE void resetEngineHours();

public slots:
    /**
     * @brief Publishes RPM CAN frame.
     *
     * Encodes RPM into a CAN-like payload
     * and publishes it over ZMQ.
     *
     * @param rpm UI-scaled RPM value (e.g. 10, 20, 30)
     */
    void publishRPM(int rpm);

    /**
     * @brief Publishes telltale ON/OFF frame.
     *
     * @param index Telltale index
     * @param state true = ON, false = OFF
     */
    void publishTelltale(int index, bool state);

    /**
     * @brief Publishes gauge level frame.
     *
     * Converts percentage value into a CAN-like
     * gauge payload and publishes over ZMQ.
     *
     * @param gaugeIndex Gauge index (Fuel, Coolant, etc.)
     * @param percent Percentage value (0–100)
     */
    void publishGauge(int gaugeIndex, int percent);

    /**
     * @brief Publishes engine hours frame.
     *
     * Encodes engine hours with 0.1 resolution
     * into a CAN-like payload.
     *
     * @param hours Engine hours value
     */
    void publishEngineHours(float hours);


    void publishFuelRate(float fuelRate);

    void publishDefRate(float value);

    void publishAvgEngineLoad(int percent);

private:
    /**
     * @brief ZMQ context used for publisher socket.
     */
    zmq::context_t m_context;

    /**
     * @brief ZMQ publisher socket.
     *
     * Publishes simulated CAN frames to subscribers.
     */
    zmq::socket_t  m_publisher;


    float m_engineHours = 0.0f;
    void loadEngineHours();
    void saveEngineHours();
};



#endif // ZMQPUBLISHER_H
