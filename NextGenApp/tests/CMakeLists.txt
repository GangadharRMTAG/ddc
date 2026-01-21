# =============================================================================
# NextGenApp Unit Tests - CMake Configuration
# =============================================================================
#
# @file CMakeLists.txt
# @brief CMake build configuration for NextGenApp unit tests.
#
# This file configures the build system for the Qt Test-based unit tests
# covering the NextGenApp C++ classes and helper functions.
#
# Test Suites:
#   - test_appinterface: Tests for AppInterface class
#   - test_clogger: Tests for cLogger singleton
#   - test_logmessagecontext: Tests for LogMessageContext class
#   - test_helpers: Tests for helper functions (mapPercent, etc.)
#
# Dependencies:
#   - Qt6 (or Qt5) with Core and Test modules
#   - ZeroMQ (libzmq) for AppInterface tests
#   - pkg-config for library detection
#
# @author Gangadhar Thalange
# @date 2026-01-13
# =============================================================================

cmake_minimum_required(VERSION 3.16)

project(NextGenAppTests LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test)

# ZeroMQ
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${ZMQ_INCLUDE_DIRS})

# ==============================================================================
# Test: AppInterface Tests
# ==============================================================================
# Tests the main application interface class that bridges C++ and QML.
# Includes property getters/setters, signal emissions, and enum validation.
add_executable(test_appinterface
    test_appinterface.cpp
    ../include/appinterface.h
    ../src/appinterface.cpp
    ../include/clogger.h
    ../include/commonlib_global.h
    ../src/clogger.cpp
)

target_link_libraries(test_appinterface
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    ${ZMQ_LIBRARIES}
)

target_compile_definitions(test_appinterface PRIVATE ORIENTATION="PORTRAIT" UNIT_TEST)

add_test(NAME AppInterfaceTests COMMAND test_appinterface)

# ==============================================================================
# Test: cLogger Tests
# ==============================================================================
# Tests the application-wide logging singleton class.
# Includes initialization, log levels, file handling, and buffer management.
add_executable(test_clogger
    test_clogger.cpp
    ../include/clogger.h
    ../include/commonlib_global.h
    ../src/clogger.cpp
)

target_link_libraries(test_clogger
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
)

add_test(NAME cLoggerTests COMMAND test_clogger)

# ==============================================================================
# Test: LogMessageContext Tests
# ==============================================================================
# Tests the log message context data class.
# Includes constructors, getters, setters, and edge cases.
add_executable(test_logmessagecontext
    test_logmessagecontext.cpp
    ../include/clogger.h
    ../include/commonlib_global.h
    ../src/clogger.cpp
)

target_link_libraries(test_logmessagecontext
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
)

add_test(NAME LogMessageContextTests COMMAND test_logmessagecontext)

# ==============================================================================
# Test: Helper Functions Tests
# ==============================================================================
# Tests static helper functions used by AppInterface.
# Includes mapPercent(), percentToLiters(), and CAN ID constants.
add_executable(test_helpers
    test_helpers.cpp
    ../include/appinterface.h
    ../src/appinterface.cpp
    ../include/clogger.h
    ../include/commonlib_global.h
    ../src/clogger.cpp
)

target_link_libraries(test_helpers
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    ${ZMQ_LIBRARIES}
)

target_compile_definitions(test_helpers PRIVATE ORIENTATION="PORTRAIT" UNIT_TEST)

add_test(NAME HelperFunctionsTests COMMAND test_helpers)

# ==============================================================================
# Combined Test Target
# ==============================================================================
# Custom target to run all unit tests with a single command.
# Usage: cmake --build . --target run_all_tests
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_appinterface test_clogger test_logmessagecontext test_helpers
    COMMENT "Running all unit tests..."
)
