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

struct cLogger::cLoggerPrivate
{
    const static QtMsgType defaultLogLevel;
    QFile logFile;
    bool echoToStdOut;
    bool enableTimeStamp;
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

cLogger &cLogger::instance()
{
    static cLogger theInstance;
    return theInstance;
}

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

int cLogger::getDbVersion()
{
    QSettings settings(ORG_NAME, APP_NAME);
    return settings.value("dbVersion", -1).toInt();
}
void cLogger::setDbVersion(int ver)
{
    QSettings settings(ORG_NAME, APP_NAME);
    settings.setValue("dbVersion",ver);
}

int cLogger::getProbeDbVersion()
{
    QSettings settings(ORG_NAME, APP_NAME);
    return settings.value("probeDbVersion", -1).toInt();
}
void cLogger::setProbeDbVersion(int ver)
{
    QSettings settings(ORG_NAME, APP_NAME);
    settings.setValue("probeDbVersion",ver);
}
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

int cLogger::previousMessageBufferSize() const
{
    return d_ptr->maxPreviousMessages;
}

QString cLogger::logFilePath() const
{
    return d_ptr->logFile.fileName();
}

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

void cLogger::setEchoToStandardOut(bool value)
{
    d_ptr->echoToStdOut = value;
}

void cLogger::setEnableTimeStamp(bool value)
{
    d_ptr->enableTimeStamp = value;
}
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

void cLogger::clearLogTypes()
{
    d_ptr->logTypes.clear();
}

void cLogger::setPreviousMessageBufferSize(int arg)
{
    d_ptr->maxPreviousMessages = arg;
}

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

LogMessageContext::LogMessageContext(QObject *parent)
{
    Q_UNUSED(parent)
    init();
}

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

LogMessageContext::LogMessageContext(const LogMessageContext &other, QObject *parent) : QObject (parent)
{
    Q_UNUSED(parent)
    _threadName = other.threadName();
    _moduleName = other.moduleName();
    _fileName = other.fileName();
    _functionName = other.functionName();
    _lineNumber = other.lineNumber();
}

void LogMessageContext::operator =(const LogMessageContext &other)
{
    _threadName = other.threadName();
    _moduleName = other.moduleName();
    _fileName = other.fileName();
    _functionName = other.functionName();
    _lineNumber = other.lineNumber();
}

LogMessageContext::~LogMessageContext()
{

}

QString LogMessageContext::threadName() const
{
    return _threadName;
}

QString LogMessageContext::moduleName() const
{
    return _moduleName;
}

QString LogMessageContext::fileName() const
{
    return _fileName;
}

QString LogMessageContext::functionName() const
{
    return _functionName;
}

qint32 LogMessageContext::lineNumber() const
{
    return _lineNumber;
}

void LogMessageContext::setThreadName(QString arg)
{
    _threadName = arg;
}

void LogMessageContext::setModuleName(QString arg)
{
    _moduleName = arg;
}

void LogMessageContext::setFileName(QString arg)
{
    _fileName = arg;
}

void LogMessageContext::setFunctionName(QString arg)
{
    _functionName = arg;
}

void LogMessageContext::setLineNumber(qint32 arg)
{
    _lineNumber = arg;
}

void LogMessageContext::init()
{
    _threadName = "";
    _moduleName = "";
    _fileName = "";
    _functionName = "";
    _lineNumber = 0;
}
