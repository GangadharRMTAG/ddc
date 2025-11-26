#ifndef CLOGGER_H
#define CLOGGER_H

#include <QObject>
#include <QLoggingCategory>
#include "commonlib_global.h"

class COMMONSHARED_EXPORT LogMessageContext : public QObject
{
    Q_OBJECT
public:
    explicit LogMessageContext(QObject *parent=0);
    explicit LogMessageContext(QString thread, QString module,
                           QString file, QString function, qint32 line,
                           QObject *parent=0);
    LogMessageContext(LogMessageContext const &other, QObject *parent=0);
    void operator =(LogMessageContext const &);
    virtual ~LogMessageContext();
    QString threadName() const;
    QString moduleName() const;
    QString fileName() const;
    QString functionName() const;
    qint32 lineNumber() const;
    void setThreadName(QString arg);
    void setModuleName(QString arg);
    void setFileName(QString arg);
    void setFunctionName(QString arg);
    void setLineNumber(qint32 arg);

private:
    void init();

private:
    QString _threadName;
    QString _moduleName;
    QString _fileName;
    QString _functionName;
    qint32 _lineNumber;
};
Q_DECLARE_METATYPE(LogMessageContext)

COMMONSHARED_EXPORT Q_DECLARE_LOGGING_CATEGORY(_alarm_)

class COMMONSHARED_EXPORT cLogger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int previousMessageBufferSize READ previousMessageBufferSize
            WRITE setPreviousMessageBufferSize)
    Q_PROPERTY(QString logFilePath READ logFilePath WRITE setLogFilePath)
public:
    static cLogger &instance();

    bool init(QString fileName);

    static void messageHandler(QtMsgType type,
                               const QMessageLogContext &context,
                               const QString &msg);
    void setLoggerLevel(QtMsgType minLevel, const QString &module = "");

    int getDbVersion();
    void setDbVersion(int ver);

    int getProbeDbVersion();
    void setProbeDbVersion(int ver);

    int previousMessageBufferSize() const;

    QString logFilePath() const;

    QList<QString> previousMessages() const;

    QList<QString> previousMessages(const QDateTime &start, int maxMessages, bool reverse) const;

    void setEchoToStandardOut(bool value);

    void setEnableTimeStamp(bool value);
    static void checkForLogFile();
    static void clearLogTypes();

signals:
    void newLogMessages(const QList<QString> &msgs);
    void postAlarmMessage(const QString &msg, const LogMessageContext &context);

public slots:

    void setPreviousMessageBufferSize(int arg);
    void setLogFilePath(QString arg);
private:
    explicit cLogger(QObject *parent = nullptr);
    virtual ~cLogger();
    cLogger(cLogger const &);
    void operator =(cLogger const &);
    struct cLoggerPrivate;
    static cLoggerPrivate *d_ptr;
};

#endif // CLOGGER_H
