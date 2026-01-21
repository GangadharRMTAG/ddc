# NextGenApp Unit Tests

**Author:** Gangadhar Thalange  
**Date:** 2026-01-13  
**Copyright:** NextGen Display Project

This directory contains comprehensive unit tests for the NextGenApp project using the Qt Test framework.

## Test Structure

| Test File | Description | Class/Functions Tested |
|-----------|-------------|----------------------|
| `test_appinterface.cpp` | AppInterface class tests | Properties, signals, slots, enums |
| `test_clogger.cpp` | cLogger singleton tests | Initialization, logging levels, file handling |
| `test_logmessagecontext.cpp` | LogMessageContext tests | Constructors, getters, setters, copy operations |
| `test_helpers.cpp` | Helper function tests | `mapPercent()`, `percentToLiters()`, CAN ID constants |

## Prerequisites

- **Qt 6** (or Qt 5) with Test module
- **CMake 3.16+**
- **libzmq** development package
- **pkg-config**

### Installing Dependencies (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y \
    qt6-base-dev \
    libqt6test6 \
    cmake \
    build-essential \
    libzmq3-dev \
    pkg-config
```

## Building and Running Tests

### Using the Test Runner Script

```bash
# Make script executable (first time only)
chmod +x run_tests.sh

# Build and run all tests
./run_tests.sh

# Clean build and run
./run_tests.sh clean

# Run with verbose output
./run_tests.sh verbose

# Build only (don't run)
./run_tests.sh build-only

# Run only (must be built first)
./run_tests.sh run-only
```

### Manual Build

```bash
# Create build directory
mkdir -p build && cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build . --parallel $(nproc)

# Run all tests
ctest --output-on-failure

# Run specific test
./test_appinterface
./test_clogger
./test_logmessagecontext
./test_helpers
```

## Test Coverage

### AppInterface Tests (60+ tests)

- **Constructor Tests**: Default initialization, property defaults
- **Property Getter Tests**: All Q_PROPERTY getters
- **Property Setter Tests**: All public slots with value changes
- **Signal Emission Tests**: Verify signals are emitted on property changes
- **Enum Tests**: Telltale, GaugeType, SafetyButton enum values
- **Vector Tests**: Telltales and gauges vector initialization

### cLogger Tests (20+ tests)

- **Singleton Tests**: Instance creation, same instance verification
- **Initialization Tests**: Valid/invalid filename handling, path traversal prevention
- **Logger Level Tests**: Debug, Warning, Critical levels
- **Message Buffer Tests**: Buffer size configuration
- **Database Version Tests**: Get/set db versions

### LogMessageContext Tests (20+ tests)

- **Constructor Tests**: Default, parameterized, copy constructors
- **Getter/Setter Tests**: All properties (thread, module, file, function, line)
- **Edge Cases**: Empty strings, long strings, special characters, boundary values

### Helper Function Tests (25+ tests)

- **mapPercent Tests**: All 8 gauge levels with boundary testing
- **percentToLiters Tests**: Conversion accuracy, clamping behavior
- **CAN ID Tests**: Constant value verification

## Test Output

Tests produce output in the following format:

```
********* Start testing of TestAppInterface *********
Config: Using QtTest library 6.x.x
PASS   : TestAppInterface::initTestCase()
PASS   : TestAppInterface::testConstructor()
PASS   : TestAppInterface::testDefaultValues()
...
Totals: XX passed, 0 failed, 0 skipped, 0 blacklisted
********* Finished testing of TestAppInterface *********
```

## Adding New Tests

1. Create a new test file: `test_<component>.cpp`
2. Add to `CMakeLists.txt`:
   ```cmake
   add_executable(test_<component>
       test_<component>.cpp
       # Add source files being tested
   )
   target_link_libraries(test_<component>
       PRIVATE
       Qt${QT_VERSION_MAJOR}::Core
       Qt${QT_VERSION_MAJOR}::Test
   )
   add_test(NAME <Component>Tests COMMAND test_<component>)
   ```
3. Follow the Qt Test pattern:
   ```cpp
   #include <QtTest/QtTest>
   
   class Test<Component> : public QObject
   {
       Q_OBJECT
   private slots:
       void initTestCase();     // Setup before all tests
       void cleanupTestCase();  // Cleanup after all tests
       void init();             // Setup before each test
       void cleanup();          // Cleanup after each test
       
       void testSomething();
   };
   
   QTEST_MAIN(Test<Component>)
   #include "test_<component>.moc"
   ```

## Continuous Integration

To run tests in CI/CD pipelines:

```yaml
# GitHub Actions example
- name: Run Unit Tests
  run: |
    cd tests
    chmod +x run_tests.sh
    ./run_tests.sh
```

## Troubleshooting

### Qt not found
Ensure Qt is installed and the `CMAKE_PREFIX_PATH` is set:
```bash
export CMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
```

### ZMQ not found
Install the ZeroMQ development package:
```bash
sudo apt-get install libzmq3-dev
```

### Tests fail to link
Ensure all required source files are listed in the test's `add_executable()` command.

