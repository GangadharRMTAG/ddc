#include "zmqpublisher.h"
#include <QByteArray>
#include <QDebug>
#include<QSettings>

/**
 * @enum GaugeType
 * @brief Enumeration of supported gauge indices.
 *
 * Used only by test publisher for clarity.
 */

enum GaugeType {
    Fuel = 0,
    Coolant = 1,
    Def = 2,
    Battery = 3,
    Hydraulic = 4
};


ZmqPublisher::ZmqPublisher(QObject *parent)
    : QObject(parent),
    m_context(1),
    m_publisher(m_context, zmq::socket_type::pub)
{
    m_publisher.bind("tcp://*:5555");
    loadEngineHours();

    qDebug() << "[ZMQ PUB] Bound to tcp://*:5555";
    qDebug() << "[ZMQ PUB] Restored Engine Hours =" << m_engineHours;
}

void ZmqPublisher::publishRPM(int rpm)
{
    uint32_t id = 0xDE000400;

    QByteArray payload(8, 0x00);

    payload[6] = (rpm >> 8) & 0xFF;
    payload[7] = rpm & 0xFF;

    QByteArray frame;
    frame.append(reinterpret_cast<char*>(&id), 4);
    frame.append(payload);

    zmq::message_t msg(frame.size());
    memcpy(msg.data(), frame.data(), frame.size());
    m_publisher.send(msg, zmq::send_flags::none);

    qDebug() << "[PUB] RPM =" << rpm;
}

void ZmqPublisher::publishTelltale(int index, bool state)
{
    uint32_t id = 0xDE001000 + index;

    QByteArray payload(8, 0x00);
    payload[7] = state ? 1 : 0;

    QByteArray frame;
    frame.append(reinterpret_cast<char*>(&id), 4);
    frame.append(payload);

    zmq::message_t msg(frame.size());
    memcpy(msg.data(), frame.data(), frame.size());
    m_publisher.send(msg, zmq::send_flags::none);

    qDebug() << "[PUB] TT" << index << "=" << state;
}


void ZmqPublisher::publishGauge(int gaugeIndex, int percent)
{

    if (percent < 0)   percent = 0;
    if (percent > 100) percent = 100;

    uint32_t id = 0xDE004000 + static_cast<uint32_t>(gaugeIndex);

    // 8-byte payload (CAN standard)
    QByteArray payload(8, 0x00);
    payload[7] = static_cast<uchar>(percent);  // % value in last byte

    QByteArray frame;
    frame.append(reinterpret_cast<const char*>(&id), sizeof(id));
    frame.append(payload);

    zmq::message_t msg(frame.size());
    memcpy(msg.data(), frame.data(), frame.size());

    m_publisher.send(msg, zmq::send_flags::none);

    qDebug() << "[PUB] Gauge" << gaugeIndex << "Level =" << percent << "%";
}
void ZmqPublisher::publishEngineHours(float hours)
{
    // Enforce monotonic increase

    if (hours < m_engineHours)
    {
        qDebug() << "[PUB] Ignored backward Engine Hours change";
        return;
    }

    if (hours > 99999.9f)

    hours = 99999.9f;

    m_engineHours = hours;

    saveEngineHours();

    uint32_t id = 0xDE005000;

    uint32_t raw = static_cast<uint32_t>(m_engineHours * 10.0f);

    QByteArray payload(8, 0x00);

    payload[4] = (raw >> 24) & 0xFF;

    payload[5] = (raw >> 16) & 0xFF;

    payload[6] = (raw >> 8)  & 0xFF;

    payload[7] = raw & 0xFF;

    QByteArray frame;

    frame.append(reinterpret_cast<char*>(&id), 4);

    frame.append(payload);

    zmq::message_t msg(frame.size());

    memcpy(msg.data(), frame.data(), frame.size());

    m_publisher.send(msg, zmq::send_flags::none);

    qDebug() << "[PUB] Engine Hours =" << m_engineHours;

}


void ZmqPublisher::messagePopup(int value)
{
    uint32_t id = 0xDE006000;
    QByteArray payload(8, 0x00);
    int raw = value;

    payload[6] = (raw >> 8) & 0xFF;
    payload[7] = raw & 0xFF;

    QByteArray frame;
    frame.append(reinterpret_cast<char*>(&id), 4);
    frame.append(payload);

    zmq::message_t msg(frame.size());
    memcpy(msg.data(), frame.data(), frame.size());
    m_publisher.send(msg, zmq::send_flags::none);

    qDebug() << "[PUB] MP =" << value;
}

void ZmqPublisher::publishFuelRate(float value)
{

    uint32_t id = 0xDE006001;


    uint16_t raw = static_cast<uint16_t>(value);

    QByteArray payload(8, 0x00);


    payload[6] = (raw >> 8) & 0xFF;
    payload[7] = raw & 0xFF;

    QByteArray frame;
    frame.append(reinterpret_cast<char*>(&id), 4);
    frame.append(payload);

    zmq::message_t msg(frame.size());
    memcpy(msg.data(), frame.data(), frame.size());
    m_publisher.send(msg, zmq::send_flags::none);

    qDebug() << "[PUB] Fuel Rate =" << QString::number(value, 'f', 1);
}

void ZmqPublisher :: publishDefRate(float value){

    uint32_t id = 0xDE006002;


    uint16_t raw = static_cast<uint16_t>(value);

    QByteArray payload(8, 0x00);


    payload[6] = (raw >> 8) & 0xFF;
    payload[7] = raw & 0xFF;

    QByteArray frame;
    frame.append(reinterpret_cast<char*>(&id), 4);
    frame.append(payload);

    zmq::message_t msg(frame.size());
    memcpy(msg.data(), frame.data(), frame.size());
    m_publisher.send(msg, zmq::send_flags::none);

    qDebug() << "[PUB] Def Rate =" << QString::number(value, 'f', 1);
}

void ZmqPublisher::publishAvgEngineLoad(int percent)
{

    if (percent < 0)   percent = 0;
    if (percent > 100) percent = 100;

    uint32_t id = 0xDE006003;

    // 8-byte payload (CAN standard)
    QByteArray payload(8, 0x00);
    payload[7] = static_cast<uchar>(percent);  // % value in last byte

    QByteArray frame;
    frame.append(reinterpret_cast<const char*>(&id), sizeof(id));
    frame.append(payload);

    zmq::message_t msg(frame.size());
    memcpy(msg.data(), frame.data(), frame.size());

    m_publisher.send(msg, zmq::send_flags::none);

    qDebug() << "[PUB] Avg Engine Load" << percent << "%";
}

#include <QSettings>

void ZmqPublisher::loadEngineHours()

{

    QSettings s("NextGen", "TestPublisher");

    m_engineHours = s.value("EngineHours", 0.0).toFloat();

    if (m_engineHours < 0.0f) m_engineHours = 0.0f;

    if (m_engineHours > 99999.9f) m_engineHours = 99999.9f;

}

void ZmqPublisher::saveEngineHours()

{

    QSettings s("NextGen", "TestPublisher");

    s.setValue("EngineHours", m_engineHours);

}

void ZmqPublisher::resetEngineHours()
{
    m_engineHours = 0.0f;

    saveEngineHours();

    uint32_t id = 0xDE005000;

    uint32_t raw = 0;

    QByteArray payload(8, 0x00);

    payload[4] = 0;

    payload[5] = 0;

    payload[6] = 0;

    payload[7] = 0;

    QByteArray frame;

    frame.append(reinterpret_cast<char*>(&id), 4);

    frame.append(payload);

    zmq::message_t msg(frame.size());

    memcpy(msg.data(), frame.data(), frame.size());

    m_publisher.send(msg, zmq::send_flags::none);

    qDebug() << "[PUB] Engine Hours RESET to 0";

}



