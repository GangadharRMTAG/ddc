#include "../include/appinterface.h"

/**
 * @file src/appinterface.cpp
 * @brief Implementation of the AppInterface class.
 *
 * This file contains the implementation details for the AppInterface
 * class which acts as the application-facing interface (for example,
 * to QML or other consumers).
 *
 */

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
AppInterface::AppInterface(QObject *parent)
    : QObject{parent}
{}
