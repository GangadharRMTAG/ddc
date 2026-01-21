/**
 * @file src/clogger.cpp
 * @brief Implementation of cLogger and LogMessageContext.
 *
 * This file contains the implementation of the cLogger singleton which
 * installs a Qt message handler, writes messages to a file, echoes to
 * stdout (optionally), keeps a buffer of recent messages, and supports
 * log file rotation. It also implements LogMessageContext which carries
 * context details about a log message (thread, module, file, function,
 * and line number).
 *
 * @date 08-Dec-2025
 * @author Gangadhar Thalange
 */

#include <QThread>
#include <QMap>
#include <QDateTime>
#include <QQueue>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMutex>
#include <iostream>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QRegularExpression>
#include "../include/clogger.h"

#ifdef DISPLAY_TIME_FOR_PROFILING
//Defined in main.cpp:main()
extern qint64 g_startTime;
#endif

// log critical messages only for ALARM category
Q_LOGGING_CATEGORY(_alarm_, "alarm.global", QtMsgType::QtCriticalMsg)

/**
 * @brief Private data for cLogger (PIMPL pattern).
 *
 * This struct stores internal state for the logger implementation:
 * file handle, echo settings, timestamping, per-module log levels,
 * buffered messages, and synchronization primitives.
 */
struct cLogger::cLoggerPrivate
{
    /// Default log level used when no explicit setting exists
    const static QtMsgType defaultLogLevel;
    /// File used for persistent logging
    QFile logFile;
    /// Whether to echo messages to stdout
    bool echoToStdOut;
    /// Whether to include time stamps in the log output
    bool enableTimeStamp;
    /// Application name used for logger identification
    QString loggerAppName = "TestLogger";

    // How big we let the log file get before we roll it over
    const static qint64 logFileRolloverSize;

    // Table of log levels, by source (Test Manager, the application, etc.)
    QMap<QString, QtMsgType> logLevels;

    // List of Log Types
    QList<QtMsgType> logTypes;

    // Buffer of previous message, maintained so that the app can request them.
    QQueue<QString> previousMessages;
    int maxPreviousMessages;
    const static int defaultMaxPreviousMessages;
    // how many messages to store up before sending them to the app.
    int logNotificationThreshold;
    const static int defaultLogNotificationThreshold;

    // Mutex to manage access from multiple threads
    QMutex logMutex;

    // Last logged message. Used to keep from spamming the same message over and over.
    QString prevMsg;
};

// Use Q_GLOBAL_STATIC for thread-safe singleton initialization
Q_GLOBAL_STATIC(QMutex, d_ptrMutex)
cLogger::cLoggerPrivate *cLogger::d_ptr = nullptr;
#ifdef QT_DEBUG
const QtMsgType cLogger::cLoggerPrivate::defaultLogLevel = QtWarningMsg;
#else
const QtMsgType cLogger::cLoggerPrivate::defaultLogLevel = QtWarningMsg;
#endif

const int cLogger::cLoggerPrivate::defaultMaxPreviousMessages = 1000;
const int cLogger::cLoggerPrivate::defaultLogNotificationThreshold = 10;
const qint64 cLogger::cLoggerPrivate::logFileRolloverSize = 2097152;//1048576;

const int levelFieldWidth = 10;
const int moduleFieldWidth = 20;
const int threadFieldWidth = 30;
// Map QtMsgType enum values to string indices (QtDebugMsg=0, QtWarningMsg=1, QtCriticalMsg=2, QtFatalMsg=3, QtInfoMsg=4)
const QStringList g_strLevelList = {"DEB","WAR","CRI","FAT","INF"};
QString logFileName = "TestLogger";

/**
 * @brief Get the global cLogger singleton instance.
 *
 * The instance is created on first use and returned by reference.
 *
 * @return Reference to the cLogger singleton
 */
cLogger &cLogger::instance()
{
    static cLogger theInstance;
    return theInstance;
}

/**
 * @brief Initialize the logger.
 *
 * Validates the supplied filename and creates internal private data when
 * required. Reads logging preferences from QSettings (ORG_NAME/APP_NAME)
 * and installs the message handler.
 *
 * @param fileName Base name of the log file (no path separators allowed)
 * @return true on success, false on validation failure
 */
bool cLogger::init(QString fileName)
{
    // Validate filename to prevent path traversal and invalid characters
    if (fileName.isEmpty()) {
        qWarning() << "cLogger::init: Empty filename provided";
        return false;
    }
    
    // Check for path traversal attempts
    if (fileName.contains("..") || fileName.contains("/") || fileName.contains("\\")) {
        qWarning() << "cLogger::init: Invalid filename contains path separators:" << fileName;
        return false;
    }
    
    // Validate filename contains only safe characters
    QRegularExpression safeFilenameRegex("^[a-zA-Z0-9._-]+$");
    if (!safeFilenameRegex.match(fileName).hasMatch()) {
        qWarning() << "cLogger::init: Filename contains invalid characters:" << fileName;
        return false;
    }
    
    // Ensure d_ptr is initialized
    QMutexLocker locker(d_ptrMutex());
    if (!d_ptr) {
        d_ptr = new cLoggerPrivate();
        d_ptr->maxPreviousMessages = d_ptr->defaultMaxPreviousMessages;
        d_ptr->logNotificationThreshold = d_ptr->defaultLogNotificationThreshold;
        checkForLogFile();
    }
    
    bool status = true;
    logFileName = fileName;
    QSettings settings(ORG_NAME, APP_NAME);

    d_ptr->echoToStdOut = true; // Making is flag true will display messages to output window

    int logDebugType = settings.value("logDebugSettings", -1).toInt();
    int logWarningType = settings.value("logWarningSettings", -1).toInt();
    int logCriticalType = settings.value("logCriticalSettings", -1).toInt();
    int logInfoType = settings.value("logInfoSettings", -1).toInt();

    d_ptr->enableTimeStamp = true;
    checkForLogFile();
    // default to Critical message
    d_ptr->logTypes.clear();
    if(logDebugType == -1 &&
            logWarningType == -1 &&
            logCriticalType == -1) {
        d_ptr->logTypes.insert(0,QtMsgType::QtCriticalMsg);
    }
    if(logDebugType == 0) {
        d_ptr->logTypes.insert(0,QtMsgType::QtDebugMsg);
    }
    if(logWarningType == 1) {
        d_ptr->logTypes.insert(1,QtMsgType::QtWarningMsg);
    }
    if(logCriticalType == 2) {
        d_ptr->logTypes.insert(2,QtMsgType::QtCriticalMsg);
    }
    if(logInfoType == 4) {
        d_ptr->logTypes.insert(4,QtMsgType::QtInfoMsg);
    }
    qInstallMessageHandler(&cLogger::messageHandler);
    return status;
}

/**
 * @brief Get the stored database version from QSettings.
 *
 * @return Stored database version or -1 if not set
 */
int cLogger::getDbVersion()
{
    QSettings settings(ORG_NAME, APP_NAME);
    return settings.value("dbVersion", -1).toInt();
}

/**
 * @brief Store the database version into QSettings.
 *
 * @param ver Database version to store
 */
void cLogger::setDbVersion(int ver)
{
    QSettings settings(ORG_NAME, APP_NAME);
    settings.setValue("dbVersion",ver);
}

/**
 * @brief Get the stored probe database version from QSettings.
 *
 * @return Stored probe database version or -1 if not set
 */
int cLogger::getProbeDbVersion()
{
    QSettings settings(ORG_NAME, APP_NAME);
    return settings.value("probeDbVersion", -1).toInt();
}

/**
 * @brief Store the probe database version into QSettings.
 *
 * @param ver Probe database version to store
 */
void cLogger::setProbeDbVersion(int ver)
{
    QSettings settings(ORG_NAME, APP_NAME);
    settings.setValue("probeDbVersion",ver);
}

/**
 * @brief Qt message handler that routes messages to the cLogger.
 *
 * This function is installed with qInstallMessageHandler. It performs:
 * - initialization checks,
 * - filtering by enabled log types,
 * - deduplication of consecutive identical messages,
 * - formatting messages with timestamp/module/thread,
 * - writing to the log file and stdout,
 * - buffering recent messages and emitting them when threshold is reached,
 * - log file rollover when the file exceeds configured size.
 *
 * @param type The Qt message type
 * @param context Contextual information (file, function, line, category)
 * @param msg The actual message text
 */
void cLogger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Ensure d_ptr is initialized (thread-safe check)
    {
        QMutexLocker initLocker(d_ptrMutex());
        if (!d_ptr) {
            // Fallback to standard output if logger not initialized
            std::cerr << "[FALLBACK] " << msg.toLocal8Bit().constData() << std::endl;
            return;
        }
    }
    
    // Bail out if the log type is not available for the message
    // Use mutex to safely check logTypes
    {
        QMutexLocker logMutexLocker(&d_ptr->logMutex);
        if(!d_ptr->logTypes.contains(type)) {
            return;
        }
    }
#ifdef PLAT_LINUX_IMX6
    const QString qstr = "PulseAudioService: pa_context_connect() failed";
#else
    const QString qstr = "QSoundEffect(qaudio): Error decoding source";
#endif
    if (msg.compare(qstr)==0) //TODO: HAck borrowed from cMessageHandler
        return;
#ifdef DISPLAY_TIME_FOR_PROFILING
    QString timestamp = QString::number((QDateTime::currentMSecsSinceEpoch()-g_startTime)/1000.0,'g');
#else
    QString timestamp = d_ptr->enableTimeStamp ? QDateTime::currentDateTimeUtc().toString("[yyyy/MM/dd HH:mm:ss.zzz]") : "";
#endif

    QString logMessage;

    // Try to figure out what module generated the message based on the filename
    QString filename(context.file);
    QString moduleName = "App";
    if(filename.contains(".qml", Qt::CaseInsensitive)) {
        // special case - QML files are always the app
        moduleName = "QML";
    } else {
        //TODO:Module wise logs
//        foreach(const QString &k, d_ptr->logLevels.keys()) {
//            QString filenameSearchText = k.simplified();
//            filenameSearchText.replace(" ", "");
//            if(filename.contains(filenameSearchText, Qt::CaseInsensitive)) {
//                moduleName = k;
//                break;
//            }
//        }
    }
//Module TODO
//    // Bail out if the log level for the module that generated this message is
//    // above that of the message itself.
//    if(d_ptr->logLevels.keys().contains(moduleName)) {
//        if(type < d_ptr->logLevels[moduleName]) {
//            return;
//        }
//    }

//    moduleName = QString("[%1]").arg(moduleName).leftJustified(moduleFieldWidth, ' ');
    QString threadName = QThread::currentThread()->objectName();

#ifdef PLAT_LINUX_IMX6
    if(type == QtCriticalMsg) {
        if(QString(context.category).compare("alarm.global")==0) {
            // send alarm to cloud but DO NOT log in to Mozart.log
            QString file = QString(context.file);
            QString function = QString(context.function);
            qint32 line = context.line;
            LogMessageContext msgContext(threadName, moduleName, file, function, line);
            emit cLogger::instance().postAlarmMessage(msg, msgContext);
            return;
        }
    }
#endif

    // Safely get level string with bounds checking
    QString levelString = "UNK";
    if (type >= 0 && type < g_strLevelList.size()) {
        levelString = g_strLevelList.at(type);
    } else {
        qWarning() << "cLogger::messageHandler: Invalid message type:" << type;
    }
    // Get the thread name, if one is set
    if(threadName == "")
        threadName = "NoThread";
//    threadName = QString("[%1]").arg(threadName).leftJustified(threadFieldWidth);

    // Use thread-local storage for repeated message count to ensure thread safety
    static thread_local int repeatedMessageCount = 0;
    QString repeatedMessageString = "";
    
    // Lock mutex for the rest of the function to safely access d_ptr
    QMutexLocker logMutexLocker(&d_ptr->logMutex);
    
    // Check to see if the new log message is the same as the previous one. If so, we don't
    // want to log it again.
    if(msg == d_ptr->prevMsg) {
        repeatedMessageCount++;
        return;
    } else {
        if(repeatedMessageCount > 0) {
            repeatedMessageString = QString("%1 [%2 %3 %4] %5").arg(timestamp).arg(levelString).arg(moduleName)
                    .arg(threadName).arg(QString("(previous message repeats %1 times)").arg(repeatedMessageCount));
        }
        repeatedMessageCount = 0;
        d_ptr->prevMsg = msg;
    }

    logMessage = QString("%1 [%2 %3 %4] %5").arg(timestamp).arg(levelString).arg(moduleName)
            .arg(threadName).arg(msg);
    if(type == QtFatalMsg || type == QtCriticalMsg || type == QtWarningMsg) {
        logMessage += QString(" in %1at: %2, line %3").arg(
                    QString(context.function)).arg(QString(context.file)).arg(
                    context.line);
    }
    if(!d_ptr->logFile.isOpen()) {
        // Create log fileif not open
        QString strSysDir;
#ifdef PLAT_LINUX_IMX6
        strSysDir = "/62DLP_root/SystemFiles";
#else
        strSysDir = QCoreApplication::applicationDirPath();// + "/SystemFiles";
#endif
//        QString strFilename = strSysDir +"/Logs.log";

#ifdef Q_OS_ANDROID
        QString strFilename = logFileName+".log";
#else
        QString strFilename = strSysDir + QDir::separator() + logFileName+".log";
#endif
        d_ptr->logFile.setFileName(strFilename);
        if(!d_ptr->logFile.open(QIODevice::WriteOnly| QIODevice::Append)) {
            std::cerr << "Error: Cannot write file " << d_ptr->logFile.errorString().toLocal8Bit().constData() << std::endl;
            return;
        }
        // Set file permissions: owner read/write, group read, others read
        d_ptr->logFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther);
    }
    if(d_ptr->logFile.isOpen()) {
        QTextStream logFileStream(&d_ptr->logFile);
        if(!repeatedMessageString.isEmpty())
#ifdef SUPPORT_QT6
            logFileStream << repeatedMessageString << Qt::endl;
        logFileStream << logMessage << Qt::endl;
#else
            logFileStream << repeatedMessageString << Qt::endl;
        logFileStream << logMessage << Qt::endl;
#endif
        d_ptr->logFile.flush();
    }
    if(d_ptr->echoToStdOut) {
        if(!repeatedMessageString.isEmpty())
            std::cout << repeatedMessageString.toLocal8Bit().constData() << std::endl;
        std::cout << logMessage.toLocal8Bit().constData() << std::endl;
    }
    if(!repeatedMessageString.isEmpty())
        d_ptr->previousMessages.enqueue(repeatedMessageString);
    d_ptr->previousMessages.enqueue(logMessage);
    if(d_ptr->previousMessages.size() >= d_ptr->logNotificationThreshold) {
        emit cLogger::instance().newLogMessages(d_ptr->previousMessages);
        d_ptr->previousMessages.clear();
    }

    // Roll over the log file, if necessary
    QFileInfo fileInfo(d_ptr->logFile);
    if(fileInfo.size() > d_ptr->logFileRolloverSize)
    {
        QFile rolloverFile(d_ptr->logFile.fileName() + ".old");
        if(rolloverFile.exists())
            rolloverFile.remove();
        d_ptr->logFile.close();
        QString fileName = d_ptr->logFile.fileName();   // to revert later
        d_ptr->logFile.rename(rolloverFile.fileName());
        d_ptr->logFile.setFileName(fileName);
        d_ptr->logFile.open(QIODevice::Append);
    }
}

/**
 * @brief Set minimum log level for a specific module (or globally).
 *
 * If module is empty, all existing modules' levels are set to minLevel.
 * The specified level is also appended to the list of explicit log types.
 *
 * @param minLevel Minimum QtMsgType level to enable
 * @param module Module name (empty for global)
 */
void cLogger::setLoggerLevel(QtMsgType minLevel, const QString &module)
{
    d_ptr->loggerAppName = module;
    if(module.isEmpty()) {
        // set all modules to the specified level
        foreach(const QString &k, d_ptr->logLevels.keys())
            d_ptr->logLevels[k] = minLevel;
    }
    d_ptr->logLevels[module] = minLevel;
    d_ptr->logTypes.append(minLevel);
}

/**
 * @brief Get the configured size of the previous-message buffer.
 *
 * @return Maximum number of messages retained in the buffer
 */
int cLogger::previousMessageBufferSize() const
{
    return d_ptr->maxPreviousMessages;
}

/**
 * @brief Return path of the current log file.
 *
 * @return Full path to the open log file (or empty if unset)
 */
QString cLogger::logFilePath() const
{
    return d_ptr->logFile.fileName();
}

/**
 * @brief Read the entire log file and return its lines.
 *
 * This function acquires the log mutex, opens the file for reading,
 * reads all lines into a list, then re-opens the file for append mode.
 *
 * @return List of log lines (trimmed)
 */
QList<QString> cLogger::previousMessages() const
{
    QMutexLocker logMutexLocker(&d_ptr->logMutex);
    static QList<QString> prevMsgs;
    prevMsgs.clear();
    d_ptr->logFile.close();
    d_ptr->logFile.open(QIODevice::ReadOnly);
    QString line;
    do {
        line = d_ptr->logFile.readLine();
        prevMsgs.append(line.trimmed());
    } while(!line.isEmpty());
    d_ptr->logFile.close();
    d_ptr->logFile.open(QIODevice::Append);
    return prevMsgs;
}

/**
 * @brief Read log file entries starting from a given timestamp.
 *
 * This function returns up to maxMessages either in chronological order
 * or reverse (most recent first) depending on the reverse flag. Timestamps
 * are parsed from the file's formatted lines.
 *
 * @param start Return messages after this QDateTime (UTC)
 * @param maxMessages Maximum messages to return
 * @param reverse If true, return messages in reverse order (most recent first)
 * @return List/queue of message strings matching criteria
 */
QList<QString> cLogger::previousMessages(const QDateTime &start, int maxMessages, bool reverse) const
{
    QMutexLocker logMutexLocker(&d_ptr->logMutex);
    static QQueue<QString> prevMsgs;
    prevMsgs.clear();
    d_ptr->logFile.close();
    d_ptr->logFile.open(QIODevice::ReadOnly);
    QString line;
    bool foundStart = false;
    do {
        line = d_ptr->logFile.readLine();
        QStringList token = line.split("]")[0].split("[");
        QString timestampStr;
        if(token.size() >= 2)
            timestampStr = token[1];
        else
            break;
        QDateTime timestamp = QDateTime::fromString(timestampStr, "yyyy/MM/dd HH:mm:ss.zzz");
        timestamp.setTimeZone(QTimeZone::UTC);
        foundStart = timestamp > start;
        if(reverse && !foundStart) {
            prevMsgs.append(line.trimmed());
            if(prevMsgs.size() > maxMessages)
                prevMsgs.dequeue();
        }
    } while(!foundStart && !d_ptr->logFile.atEnd());
    if(!reverse) {
        prevMsgs.append(line.trimmed());
        while(!d_ptr->logFile.atEnd() && prevMsgs.size() < maxMessages) {
            // append the previous line first before we overwrite it
            line = d_ptr->logFile.readLine();
            prevMsgs.append(line.trimmed());
        }
    }
    d_ptr->logFile.close();
    d_ptr->logFile.open(QIODevice::Append);
    return prevMsgs;
}

/**
 * @brief Enable or disable echoing log messages to standard output.
 *
 * @param value true to echo messages to stdout, false to suppress
 */
void cLogger::setEchoToStandardOut(bool value)
{
    d_ptr->echoToStdOut = value;
}

/**
 * @brief Enable or disable timestamps in logged messages.
 *
 * @param value true to include a timestamp in each message, false otherwise
 */
void cLogger::setEnableTimeStamp(bool value)
{
    d_ptr->enableTimeStamp = value;
}

/**
 * @brief Ensure the log directory exists and create it if necessary.
 *
 * Uses platform-specific directories (or QStandardPaths) to determine
 * where logs should be placed. Sets permissive permissions on the
 * created directory.
 */
void cLogger::checkForLogFile()
{
    if( QCoreApplication::instance() == nullptr) {
        qWarning() << "Error:QCoreApplication::instance() is NULL";
        return;
    }

    QString strSysDir;
#ifdef PLAT_LINUX_IMX6
    strSysDir = "/62DLP_root/SystemFiles";
#else
    // Use QStandardPaths for better portability
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if(appDataPath.isEmpty()) {
        // Fallback to application directory if standard path not available
        if(QCoreApplication::applicationDirPath().isEmpty()) {
            qWarning() << "Error:QCoreApplication::applicationDirPath() is Empty";
            return;
        }
        strSysDir = QCoreApplication::applicationDirPath() + "/SystemFiles";
    } else {
        strSysDir = appDataPath + "/Logs";
    }
#endif
    QDir systDir(strSysDir);
    if(!systDir.exists()) {
        if(!systDir.mkpath(strSysDir)) {
            qWarning() << "Error: Failed to create log directory:" << strSysDir;
            return;
        }
        // Set directory permissions
        QFile::setPermissions(strSysDir, QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | 
                              QFile::ReadGroup | QFile::ExeGroup | QFile::ReadOther | QFile::ExeOther);
    }
}

/**
 * @brief Clear the explicitly enabled log types.
 *
 * After calling this, no log types are enabled until setLoggerLevel / init
 * or other configuration re-enables them.
 */
void cLogger::clearLogTypes()
{
    d_ptr->logTypes.clear();
}

/**
 * @brief Set the maximum number of previous messages to retain.
 *
 * @param arg New maximum buffer size (number of messages)
 */
void cLogger::setPreviousMessageBufferSize(int arg)
{
    d_ptr->maxPreviousMessages = arg;
}

/**
 * @brief Set the path of the log file to use.
 *
 * This will close the existing file if open and open the new file in
 * append mode. The path is validated to be non-empty.
 *
 * @param arg Full path to the desired log file
 */
void cLogger::setLogFilePath(QString arg)
{
    // Validate path
    if (arg.isEmpty()) {
        qWarning() << "cLogger::setLogFilePath: Empty path provided";
        return;
    }
    
    QMutexLocker locker(&d_ptr->logMutex);
    if(d_ptr->logFile.isOpen()) {
        d_ptr->logFile.close();
    }
    d_ptr->logFile.setFileName(arg);
    if (!d_ptr->logFile.open(QIODevice::Append)) {
        qWarning() << "cLogger::setLogFilePath: Failed to open log file:" << arg << d_ptr->logFile.errorString();
        return;
    }
    // Set file permissions: owner read/write, group read, others read
    d_ptr->logFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther);
}

/**
 * @brief cLogger constructor.
 *
 * Initializes private data if necessary. Not responsible for installing
 * the message handler (call init() for that).
 *
 * @param parent QObject parent
 */
cLogger::cLogger(QObject *parent) : QObject(parent)
{
    QMutexLocker locker(d_ptrMutex());
    if (!d_ptr) {
        d_ptr = new cLoggerPrivate();
        d_ptr->maxPreviousMessages = d_ptr->defaultMaxPreviousMessages;
        d_ptr->logNotificationThreshold = d_ptr->defaultLogNotificationThreshold;
        checkForLogFile();
    }
}

/**
 * @brief cLogger destructor.
 *
 * Closes the log file and releases private data.
 */
cLogger::~cLogger()
{
    if (d_ptr) {
        if (d_ptr->logFile.isOpen()) {
            d_ptr->logFile.close();
        }
        delete d_ptr;
        d_ptr = nullptr;
    }
}

/**
 * @brief Default constructor for LogMessageContext.
 *
 * Initializes context members to default empty/zero values.
 *
 * @param parent QObject parent (unused)
 */
LogMessageContext::LogMessageContext(QObject *parent)
{
    Q_UNUSED(parent)
    init();
}

/**
 * @brief Parameterized constructor for LogMessageContext.
 *
 * Populates the context with given values.
 *
 * @param thread Thread name
 * @param module Module name
 * @param file Source file name
 * @param function Function name
 * @param line Line number
 * @param parent QObject parent (unused)
 */
LogMessageContext::LogMessageContext(QString thread, QString module, QString file, QString function, qint32 line, QObject *parent)
{
    Q_UNUSED(parent)
    init();
    _threadName = thread;
    _moduleName = module;
    _fileName = file;
    _functionName = function;
    _lineNumber = line;
}

/**
 * @brief Copy constructor for LogMessageContext.
 *
 * @param other Other LogMessageContext to copy from
 * @param parent QObject parent
 */
LogMessageContext::LogMessageContext(const LogMessageContext &other, QObject *parent) : QObject (parent)
{
    Q_UNUSED(parent)
    _threadName = other.threadName();
    _moduleName = other.moduleName();
    _fileName = other.fileName();
    _functionName = other.functionName();
    _lineNumber = other.lineNumber();
}

/**
 * @brief Assignment operator for LogMessageContext.
 *
 * @param other Source context to copy from
 */
void LogMessageContext::operator =(const LogMessageContext &other)
{
    _threadName = other.threadName();
    _moduleName = other.moduleName();
    _fileName = other.fileName();
    _functionName = other.functionName();
    _lineNumber = other.lineNumber();
}

/**
 * @brief Destructor for LogMessageContext.
 */
LogMessageContext::~LogMessageContext()
{

}

/**
 * @brief Get the thread name stored in the context.
 *
 * @return Thread name
 */
QString LogMessageContext::threadName() const
{
    return _threadName;
}

/**
 * @brief Get the module name stored in the context.
 *
 * @return Module name
 */
QString LogMessageContext::moduleName() const
{
    return _moduleName;
}

/**
 * @brief Get the file name stored in the context.
 *
 * @return File name
 */
QString LogMessageContext::fileName() const
{
    return _fileName;
}

/**
 * @brief Get the function name stored in the context.
 *
 * @return Function name
 */
QString LogMessageContext::functionName() const
{
    return _functionName;
}

/**
 * @brief Get the line number stored in the context.
 *
 * @return Line number
 */
qint32 LogMessageContext::lineNumber() const
{
    return _lineNumber;
}

/**
 * @brief Set the thread name in the context.
 *
 * @param arg Thread name
 */
void LogMessageContext::setThreadName(QString arg)
{
    _threadName = arg;
}

/**
 * @brief Set the module name in the context.
 *
 * @param arg Module name
 */
void LogMessageContext::setModuleName(QString arg)
{
    _moduleName = arg;
}

/**
 * @brief Set the file name in the context.
 *
 * @param arg File name
 */
void LogMessageContext::setFileName(QString arg)
{
    _fileName = arg;
}

/**
 * @brief Set the function name in the context.
 *
 * @param arg Function name
 */
void LogMessageContext::setFunctionName(QString arg)
{
    _functionName = arg;
}

/**
 * @brief Set the line number in the context.
 *
 * @param arg Line number
 */
void LogMessageContext::setLineNumber(qint32 arg)
{
    _lineNumber = arg;
}

/**
 * @brief Initialize members of LogMessageContext to safe defaults.
 *
 * Sets string members to empty and line number to 0.
 */
void LogMessageContext::init()
{
    _threadName = "";
    _moduleName = "";
    _fileName = "";
    _functionName = "";
    _lineNumber = 0;
}
