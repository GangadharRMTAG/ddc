#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <QObject>
/**
 * @file appinterface.h
 * @brief Declaration of the AppInterface class.
 *
 * This header defines AppInterface, a QObject-derived class intended to act
 * as a high-level application interface that can expose signals and slots
 * to other parts of the application (for example QML). The class currently
 * provides a constructor and placeholders for signals/slots to be added as
 * the application's needs grow.
 */

class AppInterface : public QObject
{
    Q_OBJECT
public:
    explicit AppInterface(QObject *parent = nullptr);

signals:

public slots:

private:

};

#endif // APPINTERFACE_H
