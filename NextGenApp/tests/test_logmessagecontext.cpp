/**
 * @file test_logmessagecontext.cpp
 * @brief Unit tests for the LogMessageContext class.
 *
 * This file contains comprehensive unit tests for the LogMessageContext class,
 * which encapsulates contextual information about log messages including:
 * - Thread name where the message originated
 * - Module name that produced the message
 * - Source file name
 * - Function name
 * - Line number
 *
 * The tests cover:
 * - Default and parameterized constructors
 * - Copy constructor behavior
 * - All getter methods
 * - All setter methods
 * - Assignment operator
 * - Edge cases with special characters and boundary values
 *
 * @author Gangadhar Thalange
 * @date 2026-01-13
 */

#include <QtTest/QtTest>
#include "clogger.h"

/**
 * @class TestLogMessageContext
 * @brief Test fixture for LogMessageContext unit tests.
 *
 * This class provides the test framework for validating the LogMessageContext
 * class functionality. The class is a simple data container used by cLogger
 * to carry metadata about log messages for alarm posting and message routing.
 */
class TestLogMessageContext : public QObject
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
    // Constructor Tests
    // =========================================================================

    /**
     * @brief Test default constructor initializes all fields to empty/zero.
     */
    void testDefaultConstructor();

    /**
     * @brief Test parameterized constructor sets all fields correctly.
     */
    void testParameterizedConstructor();

    /**
     * @brief Test copy constructor creates an exact copy.
     */
    void testCopyConstructor();

    // =========================================================================
    // Getter Tests
    // =========================================================================

    /**
     * @brief Test threadName() getter returns correct value.
     */
    void testThreadNameGetter();

    /**
     * @brief Test moduleName() getter returns correct value.
     */
    void testModuleNameGetter();

    /**
     * @brief Test fileName() getter returns correct value.
     */
    void testFileNameGetter();

    /**
     * @brief Test functionName() getter returns correct value.
     */
    void testFunctionNameGetter();

    /**
     * @brief Test lineNumber() getter returns correct value.
     */
    void testLineNumberGetter();

    // =========================================================================
    // Setter Tests
    // =========================================================================

    /**
     * @brief Test setThreadName() updates the thread name.
     */
    void testSetThreadName();

    /**
     * @brief Test setModuleName() updates the module name.
     */
    void testSetModuleName();

    /**
     * @brief Test setFileName() updates the file name.
     */
    void testSetFileName();

    /**
     * @brief Test setFunctionName() updates the function name.
     */
    void testSetFunctionName();

    /**
     * @brief Test setLineNumber() updates the line number.
     */
    void testSetLineNumber();

    // =========================================================================
    // Operator Tests
    // =========================================================================

    /**
     * @brief Test assignment operator copies all fields.
     */
    void testAssignmentOperator();

    // =========================================================================
    // Edge Case Tests
    // =========================================================================

    /**
     * @brief Test handling of empty strings.
     */
    void testEmptyStrings();

    /**
     * @brief Test handling of very long strings.
     */
    void testLongStrings();

    /**
     * @brief Test handling of special characters.
     */
    void testSpecialCharacters();

    /**
     * @brief Test handling of negative line numbers.
     */
    void testNegativeLineNumber();

    /**
     * @brief Test handling of zero line number.
     */
    void testZeroLineNumber();

    /**
     * @brief Test handling of maximum integer line number.
     */
    void testMaxLineNumber();
};

// =============================================================================
// Test Lifecycle Methods
// =============================================================================

void TestLogMessageContext::initTestCase()
{
    // One-time setup before all tests run
    qDebug() << "Starting LogMessageContext unit tests";
    qDebug() << "Testing Qt version:" << qVersion();
}

void TestLogMessageContext::cleanupTestCase()
{
    // One-time cleanup after all tests complete
    qDebug() << "Completed LogMessageContext unit tests";
}

// =============================================================================
// Constructor Tests
// =============================================================================

void TestLogMessageContext::testDefaultConstructor()
{
    // Test that default constructor initializes all fields to safe defaults
    // All strings should be empty, line number should be 0
    LogMessageContext context;

    QCOMPARE(context.threadName(), QString(""));
    QCOMPARE(context.moduleName(), QString(""));
    QCOMPARE(context.fileName(), QString(""));
    QCOMPARE(context.functionName(), QString(""));
    QCOMPARE(context.lineNumber(), 0);
}

void TestLogMessageContext::testParameterizedConstructor()
{
    // Test that parameterized constructor correctly sets all fields
    LogMessageContext context("MainThread", "TestModule", "test.cpp", "testFunction", 42);

    QCOMPARE(context.threadName(), QString("MainThread"));
    QCOMPARE(context.moduleName(), QString("TestModule"));
    QCOMPARE(context.fileName(), QString("test.cpp"));
    QCOMPARE(context.functionName(), QString("testFunction"));
    QCOMPARE(context.lineNumber(), 42);
}

void TestLogMessageContext::testCopyConstructor()
{
    // Test that copy constructor creates an exact copy of the original
    LogMessageContext original("WorkerThread", "CoreModule", "core.cpp", "processData", 100);
    LogMessageContext copy(original);

    // Verify all fields match the original
    QCOMPARE(copy.threadName(), original.threadName());
    QCOMPARE(copy.moduleName(), original.moduleName());
    QCOMPARE(copy.fileName(), original.fileName());
    QCOMPARE(copy.functionName(), original.functionName());
    QCOMPARE(copy.lineNumber(), original.lineNumber());
}

// =============================================================================
// Getter Tests
// =============================================================================

void TestLogMessageContext::testThreadNameGetter()
{
    // Test threadName getter with a specific value
    LogMessageContext context("TestThread", "", "", "", 0);
    QCOMPARE(context.threadName(), QString("TestThread"));
}

void TestLogMessageContext::testModuleNameGetter()
{
    // Test moduleName getter with a specific value
    LogMessageContext context("", "TestModule", "", "", 0);
    QCOMPARE(context.moduleName(), QString("TestModule"));
}

void TestLogMessageContext::testFileNameGetter()
{
    // Test fileName getter with a specific value
    LogMessageContext context("", "", "testfile.cpp", "", 0);
    QCOMPARE(context.fileName(), QString("testfile.cpp"));
}

void TestLogMessageContext::testFunctionNameGetter()
{
    // Test functionName getter with a specific value
    LogMessageContext context("", "", "", "myFunction", 0);
    QCOMPARE(context.functionName(), QString("myFunction"));
}

void TestLogMessageContext::testLineNumberGetter()
{
    // Test lineNumber getter with a specific value
    LogMessageContext context("", "", "", "", 999);
    QCOMPARE(context.lineNumber(), 999);
}

// =============================================================================
// Setter Tests
// =============================================================================

void TestLogMessageContext::testSetThreadName()
{
    // Test setThreadName modifies the thread name
    LogMessageContext context;
    context.setThreadName("NewThread");
    QCOMPARE(context.threadName(), QString("NewThread"));
}

void TestLogMessageContext::testSetModuleName()
{
    // Test setModuleName modifies the module name
    LogMessageContext context;
    context.setModuleName("NewModule");
    QCOMPARE(context.moduleName(), QString("NewModule"));
}

void TestLogMessageContext::testSetFileName()
{
    // Test setFileName modifies the file name
    LogMessageContext context;
    context.setFileName("newfile.cpp");
    QCOMPARE(context.fileName(), QString("newfile.cpp"));
}

void TestLogMessageContext::testSetFunctionName()
{
    // Test setFunctionName modifies the function name
    LogMessageContext context;
    context.setFunctionName("newFunction");
    QCOMPARE(context.functionName(), QString("newFunction"));
}

void TestLogMessageContext::testSetLineNumber()
{
    // Test setLineNumber modifies the line number
    LogMessageContext context;
    context.setLineNumber(12345);
    QCOMPARE(context.lineNumber(), 12345);
}

// =============================================================================
// Operator Tests
// =============================================================================

void TestLogMessageContext::testAssignmentOperator()
{
    // Test that assignment operator copies all fields from source to target
    LogMessageContext source("SourceThread", "SourceModule", "source.cpp", "sourceFunc", 200);
    LogMessageContext target;

    // Perform assignment
    target = source;

    // Verify all fields were copied
    QCOMPARE(target.threadName(), source.threadName());
    QCOMPARE(target.moduleName(), source.moduleName());
    QCOMPARE(target.fileName(), source.fileName());
    QCOMPARE(target.functionName(), source.functionName());
    QCOMPARE(target.lineNumber(), source.lineNumber());
}

// =============================================================================
// Edge Case Tests
// =============================================================================

void TestLogMessageContext::testEmptyStrings()
{
    // Test that empty strings are handled correctly
    // This is the typical state after default construction
    LogMessageContext context("", "", "", "", 0);

    QCOMPARE(context.threadName(), QString(""));
    QCOMPARE(context.moduleName(), QString(""));
    QCOMPARE(context.fileName(), QString(""));
    QCOMPARE(context.functionName(), QString(""));
}

void TestLogMessageContext::testLongStrings()
{
    // Test that very long strings are stored and retrieved correctly
    // This tests for potential buffer overflow issues
    QString longString(1000, 'a');
    LogMessageContext context(longString, longString, longString, longString, 0);

    QCOMPARE(context.threadName(), longString);
    QCOMPARE(context.moduleName(), longString);
    QCOMPARE(context.fileName(), longString);
    QCOMPARE(context.functionName(), longString);
}

void TestLogMessageContext::testSpecialCharacters()
{
    // Test that special characters (HTML entities, whitespace, etc.) are preserved
    // This is important for accurate log message context
    QString special = "test<>\"'&\n\t\r";
    LogMessageContext context(special, special, special, special, 0);

    QCOMPARE(context.threadName(), special);
    QCOMPARE(context.moduleName(), special);
    QCOMPARE(context.fileName(), special);
    QCOMPARE(context.functionName(), special);
}

void TestLogMessageContext::testNegativeLineNumber()
{
    // Test that negative line numbers are stored correctly
    // While not typical, the class should handle them without issue
    LogMessageContext context;
    context.setLineNumber(-1);
    QCOMPARE(context.lineNumber(), -1);
}

void TestLogMessageContext::testZeroLineNumber()
{
    // Test that zero line number is handled correctly
    // Zero is the default value and should be valid
    LogMessageContext context;
    context.setLineNumber(0);
    QCOMPARE(context.lineNumber(), 0);
}

void TestLogMessageContext::testMaxLineNumber()
{
    // Test that maximum integer value for line number is handled correctly
    // Large source files could potentially have high line numbers
    LogMessageContext context;
    context.setLineNumber(INT32_MAX);
    QCOMPARE(context.lineNumber(), INT32_MAX);
}

// =============================================================================
// Test Entry Point
// =============================================================================

QTEST_MAIN(TestLogMessageContext)
#include "test_logmessagecontext.moc"
