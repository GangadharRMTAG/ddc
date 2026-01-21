/**
 * @file test_clogger.cpp
 * @brief Unit tests for the cLogger singleton class.
 *
 * This file contains comprehensive unit tests for the cLogger class,
 * which provides application-wide logging functionality for the NextGenApp.
 *
 * The tests cover:
 * - Singleton pattern implementation
 * - Logger initialization with various filename scenarios
 * - Log level configuration for different modules
 * - Message buffer management
 * - Echo and timestamp settings
 * - Database version tracking
 * - Log file path management
 *
 * @note cLogger implements the singleton pattern with Q_GLOBAL_STATIC
 *       for thread-safe initialization.
 *
 * @author Gangadhar Thalange
 * @date 2026-01-13
 */

#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QFile>
#include <QDir>
#include "clogger.h"

/**
 * @class TestCLogger
 * @brief Test fixture for cLogger unit tests.
 *
 * This class provides the test framework for validating the cLogger
 * singleton class functionality. It uses Qt's test framework (QTest)
 * and includes a temporary directory for log file tests.
 *
 * @note The cLogger singleton persists across tests, so some tests
 *       may affect the state seen by subsequent tests.
 */
class TestCLogger : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief One-time setup before all tests run.
     */
    void initTestCase();

    /**
     * @brief One-time cleanup after all tests complete.
     */
    void cleanupTestCase();

    // =========================================================================
    // Singleton Pattern Tests
    // =========================================================================

    /**
     * @brief Verify singleton instance is returned.
     */
    void testSingletonInstance();

    /**
     * @brief Verify multiple calls return the same instance.
     */
    void testSingletonSameInstance();

    // =========================================================================
    // Initialization Tests
    // =========================================================================

    /**
     * @brief Test initialization with a valid filename.
     */
    void testInitWithValidFilename();

    /**
     * @brief Test initialization with an empty filename (should fail).
     */
    void testInitWithEmptyFilename();

    /**
     * @brief Test initialization with path traversal attempt (should fail).
     */
    void testInitWithPathTraversal();

    /**
     * @brief Test initialization with invalid characters (should fail).
     */
    void testInitWithInvalidCharacters();

    /**
     * @brief Test initialization with slash characters (should fail).
     */
    void testInitWithSlashes();

    // =========================================================================
    // Logger Level Tests
    // =========================================================================

    /**
     * @brief Test setting debug log level for a module.
     */
    void testSetLoggerLevelDebug();

    /**
     * @brief Test setting warning log level for a module.
     */
    void testSetLoggerLevelWarning();

    /**
     * @brief Test setting critical log level for a module.
     */
    void testSetLoggerLevelCritical();

    /**
     * @brief Test setting log level with a specific module name.
     */
    void testSetLoggerLevelWithModule();

    // =========================================================================
    // Message Buffer Tests
    // =========================================================================

    /**
     * @brief Test getting the default previous message buffer size.
     */
    void testPreviousMessageBufferSize();

    /**
     * @brief Test setting a custom previous message buffer size.
     */
    void testSetPreviousMessageBufferSize();

    // =========================================================================
    // Echo Settings Tests
    // =========================================================================

    /**
     * @brief Test enabling/disabling echo to standard output.
     */
    void testSetEchoToStandardOut();

    /**
     * @brief Test enabling/disabling timestamps in log output.
     */
    void testSetEnableTimeStamp();

    // =========================================================================
    // Database Version Tests
    // =========================================================================

    /**
     * @brief Test getting the default database version.
     */
    void testGetDbVersionDefault();

    /**
     * @brief Test setting and getting the database version.
     */
    void testSetAndGetDbVersion();

    /**
     * @brief Test getting the default probe database version.
     */
    void testGetProbeDbVersionDefault();

    /**
     * @brief Test setting and getting the probe database version.
     */
    void testSetAndGetProbeDbVersion();

    // =========================================================================
    // Log File Tests
    // =========================================================================

    /**
     * @brief Test getting the current log file path.
     */
    void testLogFilePath();

    /**
     * @brief Test setting a custom log file path.
     */
    void testSetLogFilePath();

    /**
     * @brief Test setting an empty log file path (should fail gracefully).
     */
    void testSetLogFilePathEmpty();

    // =========================================================================
    // Clear Log Types Tests
    // =========================================================================

    /**
     * @brief Test clearing all enabled log types.
     */
    void testClearLogTypes();

private:
    /**
     * @brief Temporary directory for log file tests.
     *
     * This directory is automatically created and cleaned up by Qt.
     */
    QTemporaryDir m_tempDir;
};

// =============================================================================
// Test Lifecycle Methods
// =============================================================================

void TestCLogger::initTestCase()
{
    // One-time setup before all tests run
    qDebug() << "Starting cLogger unit tests";
    qDebug() << "Testing Qt version:" << qVersion();

    // Verify temporary directory is valid for file tests
    QVERIFY(m_tempDir.isValid());
}

void TestCLogger::cleanupTestCase()
{
    // One-time cleanup after all tests complete
    qDebug() << "Completed cLogger unit tests";
}

// =============================================================================
// Singleton Pattern Tests
// =============================================================================

void TestCLogger::testSingletonInstance()
{
    // Verify that instance() returns a valid reference
    cLogger& logger = cLogger::instance();
    QVERIFY(&logger != nullptr);
}

void TestCLogger::testSingletonSameInstance()
{
    // Verify that multiple calls to instance() return the same object
    // This is the fundamental requirement of the singleton pattern
    cLogger& logger1 = cLogger::instance();
    cLogger& logger2 = cLogger::instance();
    QCOMPARE(&logger1, &logger2);
}

// =============================================================================
// Initialization Tests
// =============================================================================

void TestCLogger::testInitWithValidFilename()
{
    // Test that init() succeeds with a valid alphanumeric filename
    cLogger& logger = cLogger::instance();
    bool result = logger.init("ValidTestLog");
    QVERIFY(result);
}

void TestCLogger::testInitWithEmptyFilename()
{
    // Test that init() fails with an empty filename
    // This protects against creating logs with no name
    cLogger& logger = cLogger::instance();
    bool result = logger.init("");
    QVERIFY(!result);
}

void TestCLogger::testInitWithPathTraversal()
{
    // Test that init() rejects path traversal attempts
    // This is a security measure to prevent writing outside the log directory
    cLogger& logger = cLogger::instance();
    bool result = logger.init("../../../etc/passwd");
    QVERIFY(!result);
}

void TestCLogger::testInitWithInvalidCharacters()
{
    // Test that init() rejects filenames with special characters
    // Only alphanumeric, dots, underscores, and hyphens are allowed
    cLogger& logger = cLogger::instance();
    bool result = logger.init("test<>file");
    QVERIFY(!result);
}

void TestCLogger::testInitWithSlashes()
{
    // Test that init() rejects filenames containing path separators
    // Filenames should not contain directory components
    cLogger& logger = cLogger::instance();
    bool result = logger.init("path/to/file");
    QVERIFY(!result);
}

// =============================================================================
// Logger Level Tests
// =============================================================================

void TestCLogger::testSetLoggerLevelDebug()
{
    // Test setting debug level for a module
    // Debug is the most verbose logging level
    cLogger& logger = cLogger::instance();
    logger.setLoggerLevel(QtDebugMsg, "TestModule");
    QVERIFY(true);  // Verify no exception thrown
}

void TestCLogger::testSetLoggerLevelWarning()
{
    // Test setting warning level for a module
    // Warning level shows warnings and above (critical, fatal)
    cLogger& logger = cLogger::instance();
    logger.setLoggerLevel(QtWarningMsg, "TestModule");
    QVERIFY(true);  // Verify no exception thrown
}

void TestCLogger::testSetLoggerLevelCritical()
{
    // Test setting critical level for a module
    // Critical level shows only critical and fatal messages
    cLogger& logger = cLogger::instance();
    logger.setLoggerLevel(QtCriticalMsg, "TestModule");
    QVERIFY(true);  // Verify no exception thrown
}

void TestCLogger::testSetLoggerLevelWithModule()
{
    // Test setting log level for a specific named module
    // Each module can have its own logging threshold
    cLogger& logger = cLogger::instance();
    logger.setLoggerLevel(QtWarningMsg, "SpecificModule");
    QVERIFY(true);  // Verify no exception thrown
}

// =============================================================================
// Message Buffer Tests
// =============================================================================

void TestCLogger::testPreviousMessageBufferSize()
{
    // Test that the default buffer size is 1000 messages
    cLogger& logger = cLogger::instance();
    int size = logger.previousMessageBufferSize();
    QCOMPARE(size, 1000);
}

void TestCLogger::testSetPreviousMessageBufferSize()
{
    // Test setting a custom buffer size
    cLogger& logger = cLogger::instance();
    logger.setPreviousMessageBufferSize(500);
    QCOMPARE(logger.previousMessageBufferSize(), 500);

    // Reset to default for subsequent tests
    logger.setPreviousMessageBufferSize(1000);
}

// =============================================================================
// Echo Settings Tests
// =============================================================================

void TestCLogger::testSetEchoToStandardOut()
{
    // Test enabling and disabling echo to stdout
    // When enabled, log messages are also printed to console
    cLogger& logger = cLogger::instance();
    logger.setEchoToStandardOut(true);
    QVERIFY(true);  // Verify no exception thrown

    logger.setEchoToStandardOut(false);
    QVERIFY(true);  // Verify no exception thrown
}

void TestCLogger::testSetEnableTimeStamp()
{
    // Test enabling and disabling timestamps in log messages
    // Timestamps are in format [yyyy/MM/dd HH:mm:ss.zzz]
    cLogger& logger = cLogger::instance();
    logger.setEnableTimeStamp(true);
    QVERIFY(true);  // Verify no exception thrown

    logger.setEnableTimeStamp(false);
    QVERIFY(true);  // Verify no exception thrown
}

// =============================================================================
// Database Version Tests
// =============================================================================

void TestCLogger::testGetDbVersionDefault()
{
    // Test getting database version (defaults to -1 if not set)
    // Database version is stored in QSettings
    cLogger& logger = cLogger::instance();
    int version = logger.getDbVersion();
    QVERIFY(version >= -1);
}

void TestCLogger::testSetAndGetDbVersion()
{
    // Test setting and retrieving the database version
    cLogger& logger = cLogger::instance();
    logger.setDbVersion(42);
    QCOMPARE(logger.getDbVersion(), 42);
}

void TestCLogger::testGetProbeDbVersionDefault()
{
    // Test getting probe database version (defaults to -1 if not set)
    cLogger& logger = cLogger::instance();
    int version = logger.getProbeDbVersion();
    QVERIFY(version >= -1);
}

void TestCLogger::testSetAndGetProbeDbVersion()
{
    // Test setting and retrieving the probe database version
    cLogger& logger = cLogger::instance();
    logger.setProbeDbVersion(99);
    QCOMPARE(logger.getProbeDbVersion(), 99);
}

// =============================================================================
// Log File Tests
// =============================================================================

void TestCLogger::testLogFilePath()
{
    // Test getting the current log file path
    // Path may be empty if logging hasn't started, or contain .log extension
    cLogger& logger = cLogger::instance();
    QString path = logger.logFilePath();
    QVERIFY(path.isEmpty() || path.contains(".log") || path.contains("Log"));
}

void TestCLogger::testSetLogFilePath()
{
    // Test setting a custom log file path
    cLogger& logger = cLogger::instance();
    QString testPath = m_tempDir.path() + "/test_log.log";
    logger.setLogFilePath(testPath);
    QCOMPARE(logger.logFilePath(), testPath);
}

void TestCLogger::testSetLogFilePathEmpty()
{
    // Test that setting an empty path is rejected
    // The current path should remain unchanged
    cLogger& logger = cLogger::instance();
    QString originalPath = logger.logFilePath();
    logger.setLogFilePath("");
    QCOMPARE(logger.logFilePath(), originalPath);
}

// =============================================================================
// Clear Log Types Tests
// =============================================================================

void TestCLogger::testClearLogTypes()
{
    // Test clearing all enabled log types
    // After clearing, no messages will be logged until levels are re-enabled
    cLogger::clearLogTypes();
    QVERIFY(true);  // Verify no crash
}

// =============================================================================
// Test Entry Point
// =============================================================================

QTEST_MAIN(TestCLogger)
#include "test_clogger.moc"
