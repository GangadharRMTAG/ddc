#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QObject>

/** CAN IDs */
/**
 * @def CAN_ID_FUEL_LEVEL
 * @brief Base CAN/ZMQ identifier for fuel level information.
 */
#define CAN_ID_FUEL_LEVEL   0xDE004000

/**
 * @def CAN_ID_BTN_BASE
 * @brief Base CAN/ZMQ identifier for button press events.
 */
#define CAN_ID_BTN_BASE    0xDE002000

/**
 * @def CAN_ID_RPM
 * @brief Base CAN/ZMQ identifier for RPM information.
 */
#define CAN_ID_RPM    0xDE000400

/**
 * @def CAN_ID_TELLTALES
 * @brief Base CAN/ZMQ identifier for telltales information.
 */
#define CAN_ID_TELLTALES    0xDE001000

/**
 * @def CAN_ID_POPUP
 * @brief Base CAN/ZMQ identifier for Message Popups.
 */
#define CAN_ID_POPUP    0xDE006000

/**
 * @def CAN_ID_ENGINEHOURS
 * @brief Base CAN/ZMQ identifier for Engine hours information.
 */
#define CAN_ID_ENGINEHOURS    0xDE005000

/**
 * @def CAN_ID_FUELRATE
 * @brief Base CAN/ZMQ identifier for fuel rate information.
 */
#define CAN_ID_FUELRATE    0xDE006001

/**
 * @def CAN_ID_DEFRATE
 * @brief Base CAN/ZMQ identifier for def rate information.
 */
#define CAN_ID_DEFRATE    0xDE006002

/**
 * @def CAN_ID_ENGINELOAD
 * @brief Base CAN/ZMQ identifier for engine load information.
 */
#define CAN_ID_ENGINELOAD    0xDE006003

/** Default values */
static constexpr const char* LOCAL_HOST_IP = "tcp://127.0.0.1:5555";
static constexpr float FUEL_TANK_CAPACITY_L = 100.0f;
static const QString LAST_RESET_DATE = "11/05/1998";
static constexpr float FUEL_USAGE = 99999.0f;
static constexpr int LEVEL1  = 12;
static constexpr int LEVEL2  = 25;
static constexpr int LEVEL3 = 37;
static constexpr int LEVEL4 = 50;
static constexpr int LEVEL5 = 62;
static constexpr int LEVEL6 = 75;
static constexpr int LEVEL7 = 87;

#endif // CONSTANTS_H
