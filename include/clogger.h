#ifndef CLOGGER_H
#define CLOGGER_H

#include <QObject>
#include <QLoggingCategory>
#include "commonlib_global.h"

/**
 * @file clogger.h
 * @brief Central logging types and the cLogger singleton declaration.
 *
 * This header declares the LogMessageContext helper type used to carry
 * contextual information about a log message (thread, module, file,
 * function and line). It also declares the cLogger singleton class
 * which provides application-wide logging facilities, including a
 * custom message handler, log file management and previous-message
 * buffering.
 */

/**
  * @class LogMessageContext
  * @brief Encapsulates context information for a log message.
  *
  * Instances of this class carry metadata about where a log message
  * originated: thread name, module name, file and function names, and
  * the line number. The class is QObject-derived so it can be used in
  * signal/slot communications and registered as a Qt metatype.
  */
class COMMONSHARED_EXPORT LogMessageContext : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs an empty LogMessageContext.
     * @param parent QObject parent (optional).
     */
    explicit LogMessageContext(QObject *parent=0);

    /**
     * @brief Constructs a LogMessageContext with the provided values.
     * @param thread Name of the thread that produced the log message.
     * @param module Logical module/component name.
     * @param file Source file name.
     * @param function Function name.
     * @param line Source line number.
     * @param parent QObject parent (optional).
     */
    explicit LogMessageContext(QString thread, QString module,
                           QString file, QString function, qint32 line,
                           QObject *parent=0);

    /**
     * @brief Copy constructor.
     * @param other Source context to copy.
     * @param parent QObject parent (optional).
     */
    LogMessageContext(LogMessageContext const &other, QObject *parent=0);

    /**
     * @brief Assignment operator.
     * @param other Source to assign from.
     */
    void operator =(LogMessageContext const &);

    /**
     * @brief Destructor.
     */
    virtual ~LogMessageContext();

    /**
     * @brief Returns the stored thread name.
     * @return Thread name as a QString.
     */
    QString threadName() const;

    /**
     * @brief Returns the stored module name.
     * @return Module name as a QString.
     */
    QString moduleName() const;

    /**
     * @brief Returns the stored source file name.
     * @return File name as a QString.
     */
    QString fileName() const;

    /**
     * @brief Returns the stored function name.
     * @return Function name as a QString.
     */
    QString functionName() const;

    /**
     * @brief Returns the stored line number.
     * @return Line number as qint32.
     */
    qint32 lineNumber() const;

    /**
     * @brief Sets the thread name.
     * @param arg New thread name.
     */
    void setThreadName(QString arg);

    /**
     * @brief Sets the module name.
     * @param arg New module name.
     */
    void setModuleName(QString arg);

    /**
     * @brief Sets the file name.
     * @param arg New file name.
     */
    void setFileName(QString arg);

    /**
     * @brief Sets the function name.
     * @param arg New function name.
     */
    void setFunctionName(QString arg);

    /**
     * @brief Sets the line number.
     * @param arg New line number.
     */
    void setLineNumber(qint32 arg);

private:
    /**
     * @brief Common initializer used by constructors.
     */
    void init();

private:
    QString _threadName;    /**< Name of the thread that produced the message */
    QString _moduleName;    /**< Logical module name */
    QString _fileName;      /**< Source file name */
    QString _functionName;  /**< Source function name */
    qint32 _lineNumber;     /**< Source line number */
};
Q_DECLARE_METATYPE(LogMessageContext)

COMMONSHARED_EXPORT Q_DECLARE_LOGGING_CATEGORY(_alarm_)

    /**
 * @class cLogger
 * @brief Application-wide logger singleton.
 *
 * cLogger implements a singleton logger that installs a Qt message handler,
 * manages log files and buffering of recent messages, and provides signals
 * for new messages and alarm posting. It exposes properties to configure the
 * previous message buffer size and the log file path.
 */
class COMMONSHARED_EXPORT cLogger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int previousMessageBufferSize READ previousMessageBufferSize
            WRITE setPreviousMessageBufferSize)
    Q_PROPERTY(QString logFilePath READ logFilePath WRITE setLogFilePath)
public:
    /**
     * @brief Returns the global cLogger instance.
     * @return Reference to the singleton instance.
     */
    static cLogger &instance();

    /**
     * @brief Initialize logger resources (e.g., open log file).
     * @param fileName Path to the log file to use.
     * @return true on successful initialization; false otherwise.
     */
    bool init(QString fileName);

    /**
     * @brief Qt message handler to route Qt log messages through cLogger.
     *
     * This function is intended to be set via qInstallMessageHandler().
     *
     * @param type The Qt message type (debug, warning, critical, fatal).
     * @param context Message context provided by Qt (file, function, line).
     * @param msg The actual message text.
     */
    static void messageHandler(QtMsgType type,
                               const QMessageLogContext &context,
                               const QString &msg);

    /**
     * @brief Set minimum logging level for a module.
     *
     * Messages below the specified minLevel for the (optional) module
     * will be ignored.
     *
     * @param minLevel Minimum Qt message severity to accept.
     * @param module Optional module name to limit the level to (empty for global).
     */
    void setLoggerLevel(QtMsgType minLevel, const QString &module = "");

    /**
     * @brief Get the database schema version used by the logger.
     * @return Integer DB version.
     */
    int getDbVersion();

    /**
     * @brief Set the database schema version used by the logger.
     * @param ver Schema version integer.
     */
    void setDbVersion(int ver);

    /**
     * @brief Get the probe database version.
     * @return Integer probe DB version.
     */
    int getProbeDbVersion();

    /**
     * @brief Set the probe database version.
     * @param ver Probe DB version integer.
     */
    void setProbeDbVersion(int ver);

    /**
     * @brief Returns the configured size of the previous-message buffer.
     * @return Number of messages the buffer holds.
     */
    int previousMessageBufferSize() const;

    /**
     * @brief Returns the configured path of the active log file.
     * @return Absolute or relative file path to the log file.
     */
    QString logFilePath() const;

    /**
     * @brief Returns a copy of the buffered previous messages.
     * @return List of previous log messages.
     */
    QList<QString> previousMessages() const;

    /**
     * @brief Returns previous messages starting at a given time.
     *
     * Retrieves up to maxMessages messages that occurred at or after start.
     *
     * @param start Start time for filtering messages.
     * @param maxMessages Maximum number of messages to return.
     * @param reverse If true, messages are returned in reverse chronological order.
     * @return List of matching log messages.
     */
    QList<QString> previousMessages(const QDateTime &start, int maxMessages, bool reverse) const;

    /**
     * @brief Enable or disable echoing log output to stdout/stderr.
     * @param value True to echo to standard output, false to suppress.
     */
    void setEchoToStandardOut(bool value);

    /**
     * @brief Enable or disable timestamps in log output.
     * @param value True to include timestamps, false to omit them.
     */
    void setEnableTimeStamp(bool value);

    /**
     * @brief Ensure a log file exists and is writable; create it if necessary.
     *
     * This is a convenience utility method intended to be called at startup.
     */
    static void checkForLogFile();

    /**
     * @brief Clears any runtime log-type filters so all messages are emitted.
     */
    static void clearLogTypes();

signals:
    /**
     * @brief Emitted when new log messages are available.
     * @param msgs List of formatted log message strings.
     */
    void newLogMessages(const QList<QString> &msgs);

    /**
     * @brief Emitted when an alarm-level message is posted.
     * @param msg Alarm message text (formatted).
     * @param context Context information describing where the alarm originated.
     */
    void postAlarmMessage(const QString &msg, const LogMessageContext &context);

public slots:

    /**
     * @brief Set the previous-message buffer size at runtime.
     * @param arg New buffer size (number of messages).
     */
    void setPreviousMessageBufferSize(int arg);

    /**
     * @brief Set the log file path used by the logger.
     * @param arg Path to the log file.
     */
    void setLogFilePath(QString arg);
private:
    /**
     * @brief Private constructor for singleton pattern.
     * @param parent QObject parent (optional).
     */
    explicit cLogger(QObject *parent = nullptr);

    /**
     * @brief Destructor.
     */
    virtual ~cLogger();

    // Disable copy construction/assignment for singleton
    cLogger(cLogger const &);
    void operator =(cLogger const &);

    struct cLoggerPrivate; /**< Opaque private data implementation */
    static cLoggerPrivate *d_ptr; /**< Pointer to private implementation */
};

#endif // CLOGGER_H
