/**
 * @file test_helpers.cpp
 * @brief Unit tests for helper functions in the NextGenApp.
 *
 * This file contains unit tests for static helper functions used by
 * the AppInterface class, including:
 * - mapPercent(): Converts percentage values to gauge display levels (1-8)
 * - percentToLiters(): Converts fuel percentage to volume in liters
 * - CAN ID constants for ZMQ message identification
 *
 * The tests cover:
 * - All gauge level boundaries for mapPercent()
 * - Edge cases including negative and overflow values
 * - Fuel tank capacity calculations
 * - CAN ID constant values
 *
 * @note Some helper functions are static in appinterface.cpp, so they are
 *       duplicated here for testing purposes. This is a common pattern when
 *       testing internal implementation details.
 *
 * @author Gangadhar Thalange
 * @date 2026-01-13
 */

#include <QtTest/QtTest>
#include "appinterface.h"
#include "constants.h"


// =============================================================================
// Local copies of static helper functions for testing
// =============================================================================

/**
 * @brief Maps a percentage value to a gauge display level (1-8).
 *
 * This is a local copy of the static function from appinterface.cpp
 * for testing purposes. The original function is not accessible outside
 * its translation unit.
 *
 * Mapping thresholds:
 *  - 0–12%   -> Level 1 (empty/low)
 *  - 13–25%  -> Level 2
 *  - 26–37%  -> Level 3
 *  - 38–50%  -> Level 4
 *  - 51–62%  -> Level 5
 *  - 63–75%  -> Level 6
 *  - 76–87%  -> Level 7
 *  - 88–100% -> Level 8 (full)
 *
 * @param percent Percentage value (0–100, though values outside range are handled)
 * @return Gauge display level (1–8)
 */
static int mapPercent(int percent)
{
    if (percent <= 12)  return 1;
    if (percent <= 25)  return 2;
    if (percent <= 37)  return 3;
    if (percent <= 50)  return 4;
    if (percent <= 62)  return 5;
    if (percent <= 75)  return 6;
    if (percent <= 87)  return 7;
    return 8; // 88–100
}


/**
 * @brief Converts fuel percentage to volume in liters.
 *
 * This is a local copy of the static function from appinterface.cpp
 * for testing purposes. Values outside 0-100 are clamped to valid range.
 *
 * @param percent Fuel level percentage (0-100)
 * @return Fuel volume in liters (0.0 - FUEL_TANK_CAPACITY_L)
 */
static float percentToLiters(int percent)
{
    // Clamp percentage to valid range [0, 100]
    percent = qBound(0, percent, 100);
    return (percent / 100.0f) * FUEL_TANK_CAPACITY_L;
}

/**
 * @class TestHelpers
 * @brief Test fixture for helper function unit tests.
 *
 * This class provides the test framework for validating helper functions
 * used by the AppInterface class. These functions handle data conversion
 * and are critical for correct gauge display in the UI.
 */
class TestHelpers : public QObject
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
    // mapPercent Function Tests - Level 1 (0-12%)
    // =========================================================================

    /**
     * @brief Test mapPercent returns level 1 at lower bound (0%).
     */
    void testMapPercentLevel1_LowerBound();

    /**
     * @brief Test mapPercent returns level 1 at upper bound (12%).
     */
    void testMapPercentLevel1_UpperBound();

    // =========================================================================
    // mapPercent Function Tests - Level 2 (13-25%)
    // =========================================================================

    /**
     * @brief Test mapPercent returns level 2 at lower bound (13%).
     */
    void testMapPercentLevel2_LowerBound();

    /**
     * @brief Test mapPercent returns level 2 at upper bound (25%).
     */
    void testMapPercentLevel2_UpperBound();

    // =========================================================================
    // mapPercent Function Tests - Level 3 (26-37%)
    // =========================================================================

    /**
     * @brief Test mapPercent returns level 3 at lower bound (26%).
     */
    void testMapPercentLevel3_LowerBound();

    /**
     * @brief Test mapPercent returns level 3 at upper bound (37%).
     */
    void testMapPercentLevel3_UpperBound();

    // =========================================================================
    // mapPercent Function Tests - Level 4 (38-50%)
    // =========================================================================

    /**
     * @brief Test mapPercent returns level 4 at lower bound (38%).
     */
    void testMapPercentLevel4_LowerBound();

    /**
     * @brief Test mapPercent returns level 4 at upper bound (50%).
     */
    void testMapPercentLevel4_UpperBound();

    // =========================================================================
    // mapPercent Function Tests - Level 5 (51-62%)
    // =========================================================================

    /**
     * @brief Test mapPercent returns level 5 at lower bound (51%).
     */
    void testMapPercentLevel5_LowerBound();

    /**
     * @brief Test mapPercent returns level 5 at upper bound (62%).
     */
    void testMapPercentLevel5_UpperBound();

    // =========================================================================
    // mapPercent Function Tests - Level 6 (63-75%)
    // =========================================================================

    /**
     * @brief Test mapPercent returns level 6 at lower bound (63%).
     */
    void testMapPercentLevel6_LowerBound();

    /**
     * @brief Test mapPercent returns level 6 at upper bound (75%).
     */
    void testMapPercentLevel6_UpperBound();

    // =========================================================================
    // mapPercent Function Tests - Level 7 (76-87%)
    // =========================================================================

    /**
     * @brief Test mapPercent returns level 7 at lower bound (76%).
     */
    void testMapPercentLevel7_LowerBound();

    /**
     * @brief Test mapPercent returns level 7 at upper bound (87%).
     */
    void testMapPercentLevel7_UpperBound();

    // =========================================================================
    // mapPercent Function Tests - Level 8 (88-100%)
    // =========================================================================

    /**
     * @brief Test mapPercent returns level 8 at lower bound (88%).
     */
    void testMapPercentLevel8_LowerBound();

    /**
     * @brief Test mapPercent returns level 8 at upper bound (100%).
     */
    void testMapPercentLevel8_UpperBound();

    // =========================================================================
    // mapPercent Edge Case Tests
    // =========================================================================

    /**
     * @brief Test mapPercent with zero input.
     */
    void testMapPercentZero();

    /**
     * @brief Test mapPercent with negative input values.
     */
    void testMapPercentNegative();

    /**
     * @brief Test mapPercent with exactly 100%.
     */
    void testMapPercentHundred();

    /**
     * @brief Test mapPercent with values over 100%.
     */
    void testMapPercentOverHundred();

    // =========================================================================
    // percentToLiters Function Tests
    // =========================================================================

    /**
     * @brief Test percentToLiters with 0% returns 0 liters.
     */
    void testPercentToLitersZero();

    /**
     * @brief Test percentToLiters with 50% returns 50 liters.
     */
    void testPercentToLitersFifty();

    /**
     * @brief Test percentToLiters with 100% returns tank capacity.
     */
    void testPercentToLitersHundred();

    /**
     * @brief Test percentToLiters clamps negative values to 0.
     */
    void testPercentToLitersNegative();

    /**
     * @brief Test percentToLiters clamps values over 100 to capacity.
     */
    void testPercentToLitersOverHundred();

    /**
     * @brief Test percentToLiters with various boundary values.
     */
    void testPercentToLitersBoundaryValues();

    // =========================================================================
    // CAN ID Constant Tests
    // =========================================================================

    /**
     * @brief Verify CAN_ID_FUEL_LEVEL constant value.
     */
    void testCanIdFuelLevel();

    /**
     * @brief Verify CAN_ID_BTN_BASE constant value.
     */
    void testCanIdBtnBase();
};

// =============================================================================
// Test Lifecycle Methods
// =============================================================================

void TestHelpers::initTestCase()
{
    // One-time setup before all tests run
    qDebug() << "Starting helper function unit tests";
    qDebug() << "Testing Qt version:" << qVersion();
    qDebug() << "Fuel tank capacity:" << FUEL_TANK_CAPACITY_L << "liters";
}

void TestHelpers::cleanupTestCase()
{
    // One-time cleanup after all tests complete
    qDebug() << "Completed helper function unit tests";
}

// =============================================================================
// mapPercent Function Tests - Level 1 (0-12%)
// =============================================================================

void TestHelpers::testMapPercentLevel1_LowerBound()
{
    // Test that 0% maps to gauge level 1 (lowest/empty)
    QCOMPARE(mapPercent(0), 1);
}

void TestHelpers::testMapPercentLevel1_UpperBound()
{
    // Test that 12% is the maximum value that still maps to level 1
    QCOMPARE(mapPercent(12), 1);
}

// =============================================================================
// mapPercent Function Tests - Level 2 (13-25%)
// =============================================================================

void TestHelpers::testMapPercentLevel2_LowerBound()
{
    // Test that 13% is the minimum value that maps to level 2
    QCOMPARE(mapPercent(13), 2);
}

void TestHelpers::testMapPercentLevel2_UpperBound()
{
    // Test that 25% is the maximum value that still maps to level 2
    QCOMPARE(mapPercent(25), 2);
}

// =============================================================================
// mapPercent Function Tests - Level 3 (26-37%)
// =============================================================================

void TestHelpers::testMapPercentLevel3_LowerBound()
{
    // Test that 26% is the minimum value that maps to level 3
    QCOMPARE(mapPercent(26), 3);
}

void TestHelpers::testMapPercentLevel3_UpperBound()
{
    // Test that 37% is the maximum value that still maps to level 3
    QCOMPARE(mapPercent(37), 3);
}

// =============================================================================
// mapPercent Function Tests - Level 4 (38-50%)
// =============================================================================

void TestHelpers::testMapPercentLevel4_LowerBound()
{
    // Test that 38% is the minimum value that maps to level 4
    QCOMPARE(mapPercent(38), 4);
}

void TestHelpers::testMapPercentLevel4_UpperBound()
{
    // Test that 50% is the maximum value that still maps to level 4
    QCOMPARE(mapPercent(50), 4);
}

// =============================================================================
// mapPercent Function Tests - Level 5 (51-62%)
// =============================================================================

void TestHelpers::testMapPercentLevel5_LowerBound()
{
    // Test that 51% is the minimum value that maps to level 5
    QCOMPARE(mapPercent(51), 5);
}

void TestHelpers::testMapPercentLevel5_UpperBound()
{
    // Test that 62% is the maximum value that still maps to level 5
    QCOMPARE(mapPercent(62), 5);
}

// =============================================================================
// mapPercent Function Tests - Level 6 (63-75%)
// =============================================================================

void TestHelpers::testMapPercentLevel6_LowerBound()
{
    // Test that 63% is the minimum value that maps to level 6
    QCOMPARE(mapPercent(63), 6);
}

void TestHelpers::testMapPercentLevel6_UpperBound()
{
    // Test that 75% is the maximum value that still maps to level 6
    QCOMPARE(mapPercent(75), 6);
}

// =============================================================================
// mapPercent Function Tests - Level 7 (76-87%)
// =============================================================================

void TestHelpers::testMapPercentLevel7_LowerBound()
{
    // Test that 76% is the minimum value that maps to level 7
    QCOMPARE(mapPercent(76), 7);
}

void TestHelpers::testMapPercentLevel7_UpperBound()
{
    // Test that 87% is the maximum value that still maps to level 7
    QCOMPARE(mapPercent(87), 7);
}

// =============================================================================
// mapPercent Function Tests - Level 8 (88-100%)
// =============================================================================

void TestHelpers::testMapPercentLevel8_LowerBound()
{
    // Test that 88% is the minimum value that maps to level 8 (full)
    QCOMPARE(mapPercent(88), 8);
}

void TestHelpers::testMapPercentLevel8_UpperBound()
{
    // Test that 100% maps to gauge level 8 (full)
    QCOMPARE(mapPercent(100), 8);
}

// =============================================================================
// mapPercent Edge Case Tests
// =============================================================================

void TestHelpers::testMapPercentZero()
{
    // Test explicit zero input (empty tank)
    QCOMPARE(mapPercent(0), 1);
}

void TestHelpers::testMapPercentNegative()
{
    // Test that negative values are handled gracefully
    // They should map to level 1 (treated as 0 or below)
    QCOMPARE(mapPercent(-1), 1);
    QCOMPARE(mapPercent(-100), 1);
}

void TestHelpers::testMapPercentHundred()
{
    // Test exactly 100% (full tank)
    QCOMPARE(mapPercent(100), 8);
}

void TestHelpers::testMapPercentOverHundred()
{
    // Test that values over 100% are capped at level 8
    // This handles sensor errors or data glitches gracefully
    QCOMPARE(mapPercent(101), 8);
    QCOMPARE(mapPercent(200), 8);
}

// =============================================================================
// percentToLiters Function Tests
// =============================================================================

void TestHelpers::testPercentToLitersZero()
{
    // Test that 0% returns 0 liters (empty tank)
    QCOMPARE(percentToLiters(0), 0.0f);
}

void TestHelpers::testPercentToLitersFifty()
{
    // Test that 50% returns 50 liters (half tank)
    QCOMPARE(percentToLiters(50), 50.0f);
}

void TestHelpers::testPercentToLitersHundred()
{
    // Test that 100% returns full tank capacity
    QCOMPARE(percentToLiters(100), 100.0f);
}

void TestHelpers::testPercentToLitersNegative()
{
    // Test that negative values are clamped to 0 liters
    // qBound prevents negative fuel calculations
    QCOMPARE(percentToLiters(-10), 0.0f);
    QCOMPARE(percentToLiters(-100), 0.0f);
}

void TestHelpers::testPercentToLitersOverHundred()
{
    // Test that values over 100% are clamped to tank capacity
    // qBound prevents impossible fuel calculations
    QCOMPARE(percentToLiters(110), 100.0f);
    QCOMPARE(percentToLiters(200), 100.0f);
}

void TestHelpers::testPercentToLitersBoundaryValues()
{
    // Test various boundary values within valid range
    QCOMPARE(percentToLiters(1), 1.0f);
    QCOMPARE(percentToLiters(25), 25.0f);
    QCOMPARE(percentToLiters(75), 75.0f);
    QCOMPARE(percentToLiters(99), 99.0f);
}

// =============================================================================
// CAN ID Constant Tests
// =============================================================================

void TestHelpers::testCanIdFuelLevel()
{
    // Verify CAN_ID_FUEL_LEVEL has the correct hex value
    // This is the base ID for gauge-related ZMQ messages
    QCOMPARE(static_cast<uint32_t>(CAN_ID_FUEL_LEVEL), 0xDE004000u);
}

void TestHelpers::testCanIdBtnBase()
{
    // Verify CAN_ID_BTN_BASE has the correct hex value
    // This is the base ID for button event ZMQ messages
    QCOMPARE(static_cast<uint32_t>(CAN_ID_BTN_BASE), 0xDE002000u);
}

// =============================================================================
// Test Entry Point
// =============================================================================

QTEST_MAIN(TestHelpers)
#include "test_helpers.moc"
