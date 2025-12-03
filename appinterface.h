#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <QObject>

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
