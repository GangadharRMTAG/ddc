#!/bin/bash
# =============================================================================
# run_tests.sh - Build and run all unit tests for NextGenApp
# =============================================================================
#
# @file run_tests.sh
# @brief Shell script to build and execute NextGenApp unit tests.
#
# This script automates the process of building and running the Qt Test-based
# unit tests for the NextGenApp project. It handles dependency checking,
# Qt path detection, CMake configuration, and test execution.
#
# Usage:
#   ./run_tests.sh          - Build and run all tests
#   ./run_tests.sh clean    - Clean build directory and rebuild
#   ./run_tests.sh verbose  - Run tests with verbose output
#   ./run_tests.sh build-only - Only build tests, don't run
#   ./run_tests.sh run-only   - Only run tests (must be built first)
#   ./run_tests.sh help       - Show help message
#
# Requirements:
#   - Qt6 (or Qt5) with Test module
#   - CMake 3.16+
#   - libzmq development package
#   - pkg-config
#
# @author Gangadhar Thalange
# @date 2026-01-13
# =============================================================================
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  $1${NC}"
    echo -e "${BLUE}========================================${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

# Check dependencies
check_dependencies() {
    print_header "Checking Dependencies"
    
    # Check CMake
    if command -v cmake &> /dev/null; then
        CMAKE_VERSION=$(cmake --version | head -n1)
        print_success "CMake found: $CMAKE_VERSION"
    else
        print_error "CMake not found. Please install CMake 3.16+"
        exit 1
    fi
    
    # Check Qt
    if pkg-config --exists Qt6Core 2>/dev/null; then
        QT_VERSION=$(pkg-config --modversion Qt6Core)
        print_success "Qt6 found: $QT_VERSION"
    elif pkg-config --exists Qt5Core 2>/dev/null; then
        QT_VERSION=$(pkg-config --modversion Qt5Core)
        print_success "Qt5 found: $QT_VERSION"
    else
        print_warning "Qt not found via pkg-config, CMake will attempt to find it"
    fi
    
    # Check ZeroMQ
    if pkg-config --exists libzmq 2>/dev/null; then
        ZMQ_VERSION=$(pkg-config --modversion libzmq)
        print_success "ZeroMQ found: $ZMQ_VERSION"
    else
        print_error "ZeroMQ not found. Please install libzmq-dev"
        exit 1
    fi
    
    echo ""
}

# Clean build directory
clean_build() {
    if [ -d "$BUILD_DIR" ]; then
        print_warning "Cleaning build directory..."
        rm -rf "$BUILD_DIR"
    fi
}

# Configure and build
build_tests() {
    print_header "Building Unit Tests"
    
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    # Detect Qt path
    QT_PATH=""
    if [ -d "/home/ubuntu/Qt/6.10.1/gcc_64" ]; then
        QT_PATH="/home/ubuntu/Qt/6.10.1/gcc_64"
    elif [ -d "$HOME/Qt/6.8.0/gcc_64" ]; then
        QT_PATH="$HOME/Qt/6.8.0/gcc_64"
    elif [ -n "$Qt6_DIR" ]; then
        QT_PATH="$Qt6_DIR"
    fi
    
    echo "Configuring with CMake..."
    if [ -n "$QT_PATH" ]; then
        echo "Using Qt from: $QT_PATH"
        cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="$QT_PATH"
    else
        cmake .. -DCMAKE_BUILD_TYPE=Debug
    fi
    
    echo ""
    echo "Building tests..."
    cmake --build . --parallel $(nproc)
    
    print_success "Build completed successfully"
    echo ""
}

# Run tests
run_tests() {
    local VERBOSE_FLAG=""
    if [ "$1" == "verbose" ]; then
        VERBOSE_FLAG="--verbose"
    fi
    
    print_header "Running Unit Tests"
    
    cd "$BUILD_DIR"
    
    # Run tests using CTest
    if ctest --output-on-failure $VERBOSE_FLAG; then
        echo ""
        print_success "All tests passed!"
    else
        echo ""
        print_error "Some tests failed!"
        exit 1
    fi
}

# Print test summary
print_summary() {
    print_header "Test Summary"
    
    cd "$BUILD_DIR"
    
    echo "Test executables built:"
    for test in test_appinterface test_clogger test_logmessagecontext test_helpers; do
        if [ -f "$test" ]; then
            print_success "  $test"
        else
            print_error "  $test (not found)"
        fi
    done
    echo ""
}

# Main
main() {
    echo ""
    print_header "NextGenApp Unit Test Runner"
    echo ""
    
    case "${1:-}" in
        clean)
            clean_build
            check_dependencies
            build_tests
            run_tests
            print_summary
            ;;
        verbose)
            check_dependencies
            build_tests
            run_tests verbose
            print_summary
            ;;
        build-only)
            check_dependencies
            build_tests
            print_summary
            ;;
        run-only)
            run_tests
            ;;
        help|--help|-h)
            echo "Usage: $0 [command]"
            echo ""
            echo "Commands:"
            echo "  (none)      Build and run all tests"
            echo "  clean       Clean build directory and rebuild"
            echo "  verbose     Run tests with verbose output"
            echo "  build-only  Only build tests, don't run"
            echo "  run-only    Only run tests (must be built first)"
            echo "  help        Show this help message"
            echo ""
            ;;
        *)
            check_dependencies
            build_tests
            run_tests
            print_summary
            ;;
    esac
}

main "$@"

