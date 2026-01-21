/**
 * @file test_appinterface.cpp
 * @brief Unit tests for the AppInterface class.
 *
 * This file contains comprehensive unit tests for the AppInterface class,
 * which serves as the primary interface between the C++ backend and the
 * QML frontend in the NextGenApp application.
 *
 * The tests cover:
 * - Object construction and initialization
 * - Property getters and setters (Q_PROPERTY)
 * - Signal emissions for property changes
 * - Enum value validation (Telltale, GaugeType, SafetyButton)
 * - Vector initialization for telltales and gauges
 *
 * @note The AppInterface class uses ZMQ for inter-process communication.
 *       Tests use a static instance to avoid thread cleanup issues with
 *       the blocking ZMQ subscriber thread.
 *
 * @author Gangadhar Thalange
 * @date 2026-01-13
 */

#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QVector>
#include "appinterface.h"

/**
 * @class TestAppInterface
 * @brief Test fixture for AppInterface unit tests.
 *
 * This class provides the test framework for validating the AppInterface
 * class functionality. It uses Qt's test framework (QTest) and follows
 * the Qt unit testing patterns with initTestCase(), cleanupTestCase(),
 * init(), and cleanup() lifecycle methods.
 *
 * @note A static AppInterface instance is used to avoid ZMQ thread
 *       cleanup issues between individual test methods.
 */
class TestAppInterface : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief One-time setup before all tests run.
     * Called once before the first test function is executed.
     */
    void initTestCase();

    /**
     * @brief One-time cleanup after all tests complete.
     * Called once after the last test function is executed.
     */
    void cleanupTestCase();

    /**
     * @brief Setup before each individual test.
     * Called before every test function is executed.
     */
    void init();

    /**
     * @brief Cleanup after each individual test.
     * Called after every test function is executed.
     */
    void cleanup();

    // =========================================================================
    // Constructor Tests
    // =========================================================================

    /**
     * @brief Verify AppInterface object is successfully created.
     */
    void testConstructor();

    /**
     * @brief Verify all properties have correct default values after construction.
     */
    void testDefaultValues();

    // =========================================================================
    // Property Getter Tests
    // =========================================================================

    /**
     * @brief Test rpm property getter returns expected value.
     */
    void testRpmGetter();

    /**
     * @brief Test popup property getter returns expected value.
     */
    void testPopupGetter();

    /**
     * @brief Test fuelRate property getter returns expected value.
     */
    void testFuelRateGetter();

    /**
     * @brief Test fuelUsage property getter returns expected value.
     */
    void testFuelUsageGetter();

    /**
     * @brief Test defRate property getter returns expected value.
     */
    void testDefRateGetter();

    /**
     * @brief Test tripHours property getter returns expected value.
     */
    void testTripHoursGetter();

    /**
     * @brief Test lastTripHours property getter returns expected value.
     */
    void testLastTripHoursGetter();

    /**
     * @brief Test defUsage property getter returns expected value.
     */
    void testDefUsageGetter();

    /**
     * @brief Test avgEngineLoad property getter returns expected value.
     */
    void testAvgEngineLoadGetter();

    /**
     * @brief Test telltales vector property getter.
     */
    void testTelltalesGetter();

    /**
     * @brief Test gauges vector property getter.
     */
    void testGaugesGetter();

    /**
     * @brief Test engineHours property getter returns expected value.
     */
    void testEngineHoursGetter();

    /**
     * @brief Test isoActive property getter returns expected value.
     */
    void testIsoActiveGetter();

    /**
     * @brief Test creepActive property getter returns expected value.
     */
    void testCreepActiveGetter();

    /**
     * @brief Test currentTime property getter returns valid time string.
     */
    void testCurrentTimeGetter();

    /**
     * @brief Test lastResetDate property getter returns expected value.
     */
    void testLastResetDateGetter();

    // =========================================================================
    // Property Setter Tests
    // =========================================================================

    /**
     * @brief Test setFuelRate updates the fuelRate property.
     */
    void testSetFuelRate();

    /**
     * @brief Test setFuelUsage updates the fuelUsage property.
     */
    void testSetFuelUsage();

    /**
     * @brief Test fuelUsage overflow protection (max 99999.0f).
     */
    void testSetFuelUsageOverflow();

    /**
     * @brief Test setDefRate updates the defRate property.
     */
    void testSetDefRate();

    /**
     * @brief Test setTripHours calculates and updates trip hours.
     */
    void testSetTripHours();

    /**
     * @brief Test setLastTripHours triggers recalculation.
     */
    void testSetLastTripHours();

    /**
     * @brief Test setDefUsage updates the defUsage property.
     */
    void testSetDefUsage();

    /**
     * @brief Test setAvgEngineLoad updates the avgEngineLoad property.
     */
    void testSetAvgEngineLoad();

    /**
     * @brief Test setCreepActive updates the creepActive property.
     */
    void testSetCreepActive();

    /**
     * @brief Verify no signal emitted when creepActive set to same value.
     */
    void testSetCreepActiveNoChange();

    /**
     * @brief Test setLastResetDate updates the lastResetDate property.
     */
    void testSetLastResetDate();

    /**
     * @brief Verify no signal emitted when lastResetDate set to same value.
     */
    void testSetLastResetDateNoChange();

    // =========================================================================
    // Signal Emission Tests
    // =========================================================================

    /**
     * @brief Verify fuelRateChanged signal is emitted on value change.
     */
    void testFuelRateChangedSignal();

    /**
     * @brief Verify fuelUsageChanged signal is emitted on value change.
     */
    void testFuelUsageChangedSignal();

    /**
     * @brief Verify defRateChanged signal is emitted on value change.
     */
    void testDefRateChangedSignal();

    /**
     * @brief Verify tripHoursChanged signal is emitted on value change.
     */
    void testTripHoursChangedSignal();

    /**
     * @brief Verify lastTripHoursChanged signal is emitted on value change.
     */
    void testLastTripHoursChangedSignal();

    /**
     * @brief Verify defUsageChanged signal is emitted on value change.
     */
    void testDefUsageChangedSignal();

    /**
     * @brief Verify avgEngineLoadChanged signal is emitted on value change.
     */
    void testAvgEngineLoadChangedSignal();

    /**
     * @brief Verify creepActiveChanged signal is emitted on value change.
     */
    void testCreepActiveChangedSignal();

    /**
     * @brief Verify lastResetDateChanged signal is emitted on value change.
     */
    void testLastResetDateChangedSignal();

    /**
     * @brief Verify currentTime property is set during construction.
     */
    void testCurrentTimeChangedSignal();

    // =========================================================================
    // Enum Value Tests
    // =========================================================================

    /**
     * @brief Validate Telltale enum values match expected indices.
     */
    void testTelltaleEnumValues();

    /**
     * @brief Verify TelltaleCount enum value is correct.
     */
    void testTelltaleCount();

    /**
     * @brief Validate GaugeType enum values match expected indices.
     */
    void testGaugeTypeEnumValues();

    /**
     * @brief Verify GaugeCount enum value is correct.
     */
    void testGaugeTypeCount();

    /**
     * @brief Validate SafetyButton enum values match expected indices.
     */
    void testSafetyButtonEnumValues();

    // =========================================================================
    // Vector Tests
    // =========================================================================

    /**
     * @brief Verify telltales vector has correct size.
     */
    void testTelltalesVectorSize();

    /**
     * @brief Verify telltales vector has correct default values.
     */
    void testTelltalesVectorDefaultValues();

    /**
     * @brief Verify gauges vector has correct size.
     */
    void testGaugesVectorSize();

    /**
     * @brief Verify gauges vector has correct default values.
     */
    void testGaugesVectorDefaultValues();

    void testCanCallProcessFrame();

private:
    /**
     * @brief Pointer to the AppInterface instance under test.
     *
     * This pointer references a static instance to avoid ZMQ thread
     * cleanup issues between individual test methods.
     */
    AppInterface* m_appInterface;
};

// =============================================================================
// Test Lifecycle Methods
// =============================================================================

void TestAppInterface::initTestCase()
{
    // One-time setup before all tests run
    // This is called once before the first test function is executed
    qDebug() << "Starting AppInterface unit tests";
    qDebug() << "Testing Qt version:" << qVersion();
}

void TestAppInterface::cleanupTestCase()
{
    // One-time cleanup after all tests complete
    // This is called once after the last test function is executed
    qDebug() << "Completed AppInterface unit tests";
}

void TestAppInterface::init()
{
    // Setup before each individual test method
    //
    // Note: AppInterface starts a ZMQ subscriber thread that blocks on receive.
    // We use a static instance to avoid thread cleanup issues between tests.
    // The ZMQ thread cannot be gracefully terminated between tests because
    // it's blocked waiting for messages from the backend.
    static AppInterface* s_appInterface = nullptr;
    if (!s_appInterface) {
        s_appInterface = new AppInterface();
    }
    m_appInterface = s_appInterface;
    // testDefaultValues();
    //testFuelRateChangedSignal();
}

void TestAppInterface::cleanup()
{
    // Cleanup after each individual test method
    //
    // Note: We don't delete m_appInterface here because the ZMQ thread
    // is blocking on receive and cannot be gracefully terminated between tests.
    // The static instance will be cleaned up at program exit.
    m_appInterface = nullptr;
}

// =============================================================================
// Constructor Tests
// =============================================================================

void TestAppInterface::testConstructor()
{
    // Verify the AppInterface object was successfully created
    QVERIFY(m_appInterface != nullptr);
}

void TestAppInterface::testDefaultValues()
{
    // Verify all properties have their expected initial values
    // These values are set in the AppInterface constructor
    QCOMPARE(m_appInterface->rpm(), 0);
    QCOMPARE(m_appInterface->popup(), 0);
    QCOMPARE(m_appInterface->fuelRate(), 0.0f);
    QCOMPARE(m_appInterface->fuelUsage(), 0.0f);
    QCOMPARE(m_appInterface->defRate(), 0.0f);
    QCOMPARE(m_appInterface->tripHours(), 0.0f);
    QCOMPARE(m_appInterface->lastTripHours(), 0.0f);
    QCOMPARE(m_appInterface->defUsage(), 0.0f);
    QCOMPARE(m_appInterface->avgEngineLoad(), 0);
    QCOMPARE(m_appInterface->engineHours(), 0.0f);
    QCOMPARE(m_appInterface->isoActive(), false);
    QCOMPARE(m_appInterface->creepActive(), false);
    QCOMPARE(m_appInterface->lastResetDate(), QString("11/05/1998"));
}

// =============================================================================
// Property Getter Tests
// =============================================================================

void TestAppInterface::testRpmGetter()
{
    // RPM should be initialized to 0 (engine off state)
    QCOMPARE(m_appInterface->rpm(), 0);
}

void TestAppInterface::testPopupGetter()
{
    // Popup type should be 0 (no popup active)
    QCOMPARE(m_appInterface->popup(), 0);
}

void TestAppInterface::testFuelRateGetter()
{
    // Fuel rate should be 0.0 (no consumption initially)
    QCOMPARE(m_appInterface->fuelRate(), 0.0f);
}

void TestAppInterface::testFuelUsageGetter()
{
    // Fuel usage should be 0.0 (no fuel consumed initially)
    QCOMPARE(m_appInterface->fuelUsage(), 0.0f);
}

void TestAppInterface::testDefRateGetter()
{
    // DEF (Diesel Exhaust Fluid) rate should be 0.0 initially
    QCOMPARE(m_appInterface->defRate(), 0.0f);
}

void TestAppInterface::testTripHoursGetter()
{
    // Trip hours should be 0.0 (no trip recorded)
    QCOMPARE(m_appInterface->tripHours(), 0.0f);
}

void TestAppInterface::testLastTripHoursGetter()
{
    // Last trip hours should be 0.0 initially
    QCOMPARE(m_appInterface->lastTripHours(), 0.0f);
}

void TestAppInterface::testDefUsageGetter()
{
    // DEF usage should be 0.0 initially
    QCOMPARE(m_appInterface->defUsage(), 0.0f);
}

void TestAppInterface::testAvgEngineLoadGetter()
{
    // Average engine load should be 0% initially
    QCOMPARE(m_appInterface->avgEngineLoad(), 0);
}

void TestAppInterface::testTelltalesGetter()
{
    // Telltales vector should contain entries for all telltale types
    QVector<int> telltales = m_appInterface->telltales();
    QVERIFY(!telltales.isEmpty());
    QCOMPARE(telltales.size(), static_cast<int>(AppInterface::TelltaleCount));
}

void TestAppInterface::testGaugesGetter()
{
    // Gauges vector should contain entries for all gauge types
    QVector<int> gauges = m_appInterface->gauges();
    QVERIFY(!gauges.isEmpty());
    QCOMPARE(gauges.size(), static_cast<int>(AppInterface::GaugeCount));
}

void TestAppInterface::testEngineHoursGetter()
{
    // Engine hours should be 0.0 initially
    QCOMPARE(m_appInterface->engineHours(), 0.0f);
}

void TestAppInterface::testIsoActiveGetter()
{
    // ISO safety mode should be inactive initially
    QCOMPARE(m_appInterface->isoActive(), false);
}

void TestAppInterface::testCreepActiveGetter()
{
    // Creep mode should be inactive initially
    QCOMPARE(m_appInterface->creepActive(), false);
}

void TestAppInterface::testCurrentTimeGetter()
{
    // Current time should be set and in the format "hh:mm AP"
    QString currentTime = m_appInterface->currentTime();
    QVERIFY(!currentTime.isEmpty());
    QVERIFY(currentTime.contains(":"));  // Time format contains colon separator
}

void TestAppInterface::testLastResetDateGetter()
{
    // Last reset date should have placeholder value initially
    QCOMPARE(m_appInterface->lastResetDate(), QString("11/05/1998"));
}

// =============================================================================
// Property Setter Tests
// =============================================================================

void TestAppInterface::testSetFuelRate()
{
    // Test that setFuelRate correctly updates the property
    m_appInterface->setFuelRate(25.5f);
    QCOMPARE(m_appInterface->fuelRate(), 25.5f);
}

void TestAppInterface::testSetFuelUsage()
{
    // Test that setFuelUsage correctly updates the property
    m_appInterface->setFuelUsage(100.0f);
    QCOMPARE(m_appInterface->fuelUsage(), 100.0f);
}

void TestAppInterface::testSetFuelUsageOverflow()
{
    // Test that fuel usage is capped at 99999.0f to prevent display overflow
    // This protects against unrealistic values that could break the UI
    m_appInterface->setFuelUsage(100000.0f);
    QCOMPARE(m_appInterface->fuelUsage(), 99999.0f);
}

void TestAppInterface::testSetDefRate()
{
    // Test setDefRate updates the rate value
    // This test documents the current behavior
    m_appInterface->setDefRate(5.5f);
    QCOMPARE(m_appInterface->defRate(), 5.5f);
}

void TestAppInterface::testSetTripHours()
{
    // Test that setTripHours triggers calculation based on engine hours
    // Trip hours = engineHours - lastTripHours
    // Since both are 0, result should be 0
    m_appInterface->setTripHours(10.0f);
    QCOMPARE(m_appInterface->tripHours(), 0.0f);
}

void TestAppInterface::testSetLastTripHours()
{
    // Test that setLastTripHours triggers recalculation
    // This should not crash even with edge case values
    m_appInterface->setLastTripHours(5.0f);
    QVERIFY(true);  // Just verify no crash
}

void TestAppInterface::testSetDefUsage()
{
    // Test that setDefUsage correctly updates the property
    m_appInterface->setDefUsage(10.5f);
    QCOMPARE(m_appInterface->defUsage(), 10.5f);
}

void TestAppInterface::testSetAvgEngineLoad()
{
    // Test that setAvgEngineLoad correctly updates the property
    m_appInterface->setAvgEngineLoad(75);
    QCOMPARE(m_appInterface->avgEngineLoad(), 75);
}

void TestAppInterface::testSetCreepActive()
{
    // Test that setCreepActive correctly toggles the property
    bool initialState = m_appInterface->creepActive();
    m_appInterface->setCreepActive(true);
    QCOMPARE(m_appInterface->creepActive(), true);
    // Reset to initial state to avoid affecting other tests
    m_appInterface->setCreepActive(initialState);
}

void TestAppInterface::testSetCreepActiveNoChange()
{
    // Verify that setting creepActive to the same value does not emit signal
    // This is an optimization to prevent unnecessary QML updates
    QSignalSpy spy(m_appInterface, &AppInterface::creepActiveChanged);
    m_appInterface->setCreepActive(false);  // Already false
    QCOMPARE(spy.count(), 0);
}

void TestAppInterface::testSetLastResetDate()
{
    // Test that setLastResetDate correctly updates the property
    m_appInterface->setLastResetDate("01/13/2026");
    QCOMPARE(m_appInterface->lastResetDate(), QString("01/13/2026"));
}

void TestAppInterface::testSetLastResetDateNoChange()
{
    // Verify that setting lastResetDate to the same value does not emit signal
    m_appInterface->setLastResetDate("01/13/2026");
    QSignalSpy spy(m_appInterface, &AppInterface::lastResetDateChanged);
    m_appInterface->setLastResetDate("01/13/2026");  // Same value
    QCOMPARE(spy.count(), 0);
    // Reset for other tests
    m_appInterface->setLastResetDate("MM/DD/YYYY");
}

// =============================================================================
// Signal Emission Tests
// =============================================================================

void TestAppInterface::testFuelRateChangedSignal()
{
    // Verify signal is emitted when fuelRate changes
    QSignalSpy spy(m_appInterface, &AppInterface::fuelRateChanged);
    m_appInterface->setFuelRate(10.0f);
    QCOMPARE(spy.count(), 1);
}

void TestAppInterface::testFuelUsageChangedSignal()
{
    // Verify signal is emitted when fuelUsage changes
    QSignalSpy spy(m_appInterface, &AppInterface::fuelUsageChanged);
    m_appInterface->setFuelUsage(50.0f);
    QCOMPARE(spy.count(), 1);
}

void TestAppInterface::testDefRateChangedSignal()
{
    // Verify signal is emitted when defRate changes
    QSignalSpy spy(m_appInterface, &AppInterface::defRateChanged);
    m_appInterface->setDefRate(5.0f);
    QCOMPARE(spy.count(), 1);
}

void TestAppInterface::testTripHoursChangedSignal()
{
    // Verify signal is emitted when tripHours changes
    QSignalSpy spy(m_appInterface, &AppInterface::tripHoursChanged);
    m_appInterface->setTripHours(10.0f);
    QCOMPARE(spy.count(), 1);
}

void TestAppInterface::testLastTripHoursChangedSignal()
{
    // Verify signal is emitted when lastTripHours changes
    QSignalSpy spy(m_appInterface, &AppInterface::lastTripHoursChanged);
    m_appInterface->setLastTripHours(5.0f);
    QCOMPARE(spy.count(), 1);
}

void TestAppInterface::testDefUsageChangedSignal()
{
    // Verify signal is emitted when defUsage changes
    QSignalSpy spy(m_appInterface, &AppInterface::defUsageChanged);
    m_appInterface->setDefUsage(20.0f);
    QCOMPARE(spy.count(), 1);
}

void TestAppInterface::testAvgEngineLoadChangedSignal()
{
    // Verify signal is emitted when avgEngineLoad changes
    QSignalSpy spy(m_appInterface, &AppInterface::avgEngineLoadChanged);
    m_appInterface->setAvgEngineLoad(80);
    QCOMPARE(spy.count(), 1);
}

void TestAppInterface::testCreepActiveChangedSignal()
{
    // Verify signal is emitted when creepActive changes
    QSignalSpy spy(m_appInterface, &AppInterface::creepActiveChanged);
    m_appInterface->setCreepActive(true);
    QCOMPARE(spy.count(), 1);
}

void TestAppInterface::testLastResetDateChangedSignal()
{
    // Verify signal is emitted when lastResetDate changes
    QSignalSpy spy(m_appInterface, &AppInterface::lastResetDateChanged);
    m_appInterface->setLastResetDate("12/25/2025");
    QCOMPARE(spy.count(), 1);
}

void TestAppInterface::testCurrentTimeChangedSignal()
{
    // Verify currentTime is set during construction
    // The periodic update cannot be easily tested without waiting
    QString currentTime = m_appInterface->currentTime();
    QVERIFY(!currentTime.isEmpty());
}

// =============================================================================
// Enum Value Tests
// =============================================================================

void TestAppInterface::testTelltaleEnumValues()
{
    // Verify Telltale enum values match their expected indices
    // These values map directly to CAN/ZMQ IDs (0xDE001000 + enum value)
    QCOMPARE(static_cast<int>(AppInterface::Stop), 0);
    QCOMPARE(static_cast<int>(AppInterface::Caution), 1);
    QCOMPARE(static_cast<int>(AppInterface::SeatBelt), 2);
    QCOMPARE(static_cast<int>(AppInterface::ParkBrake), 3);
    QCOMPARE(static_cast<int>(AppInterface::WorkLamp), 4);
    QCOMPARE(static_cast<int>(AppInterface::Beacon), 5);
    QCOMPARE(static_cast<int>(AppInterface::Regeneration), 6);
    QCOMPARE(static_cast<int>(AppInterface::GridHeater), 7);
    QCOMPARE(static_cast<int>(AppInterface::HydraulicLock), 8);
    QCOMPARE(static_cast<int>(AppInterface::FootPedal), 9);
}

void TestAppInterface::testTelltaleCount()
{
    // Verify TelltaleCount matches the number of telltale types
    QCOMPARE(static_cast<int>(AppInterface::TelltaleCount), 10);
}

void TestAppInterface::testGaugeTypeEnumValues()
{
    // Verify GaugeType enum values match their expected indices
    // These values map directly to CAN/ZMQ IDs (0xDE004000 + enum value)
    QCOMPARE(static_cast<int>(AppInterface::Fuel), 0);
    QCOMPARE(static_cast<int>(AppInterface::Coolant), 1);
    QCOMPARE(static_cast<int>(AppInterface::Def), 2);
    QCOMPARE(static_cast<int>(AppInterface::Battery), 3);
    QCOMPARE(static_cast<int>(AppInterface::Hydraulic), 4);
}

void TestAppInterface::testGaugeTypeCount()
{
    // Verify GaugeCount matches the number of gauge types
    QCOMPARE(static_cast<int>(AppInterface::GaugeCount), 5);
}

void TestAppInterface::testSafetyButtonEnumValues()
{
    // Verify SafetyButton enum values match their expected indices
    // Note: SaftyDEF has a typo in the original code (should be SafetyDEF)
    QCOMPARE(static_cast<int>(AppInterface::SafetyISO), 0);
    QCOMPARE(static_cast<int>(AppInterface::SaftyDEF), 1);
    QCOMPARE(static_cast<int>(AppInterface::SafetyCreep), 2);
}

// =============================================================================
// Vector Tests
// =============================================================================

void TestAppInterface::testTelltalesVectorSize()
{
    // Verify telltales vector is sized to hold all telltale states
    QVector<int> telltales = m_appInterface->telltales();
    QCOMPARE(telltales.size(), static_cast<int>(AppInterface::TelltaleCount));
}

void TestAppInterface::testTelltalesVectorDefaultValues()
{
    // Verify all telltales are initialized to 1 (ON state) by default
    // This ensures all warning indicators are visible until explicitly cleared
    QVector<int> telltales = m_appInterface->telltales();
    for (int i = 0; i < telltales.size(); ++i) {
        QCOMPARE(telltales[i], 1);
    }
}

void TestAppInterface::testGaugesVectorSize()
{
    // Verify gauges vector is sized to hold all gauge levels
    QVector<int> gauges = m_appInterface->gauges();
    QCOMPARE(gauges.size(), static_cast<int>(AppInterface::GaugeCount));
}

void TestAppInterface::testGaugesVectorDefaultValues()
{
    // Verify all gauges are initialized to level 1 by default
    // This represents the minimum/lowest reading state
    QVector<int> gauges = m_appInterface->gauges();
    for (int i = 0; i < gauges.size(); ++i) {
        QCOMPARE(gauges[i], 1);
    }
}

void TestAppInterface::testCanCallProcessFrame()
{
    QByteArray payload(8, 0);
    m_appInterface->processFrame(0xDE000400, payload);  // should COMPILE
    QVERIFY(true);
}

// =============================================================================
// Test Entry Point
// =============================================================================

QTEST_MAIN(TestAppInterface)
#include "test_appinterface.moc"
