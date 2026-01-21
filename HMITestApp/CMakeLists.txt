cmake_minimum_required(VERSION 3.16)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
project(HMITestApp VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 REQUIRED COMPONENTS Quick Core Widgets)
qt_standard_project_setup(REQUIRES 6.8)
qt_add_executable(appHMITestApp
    main.cpp
)
# ---------------- ZeroMQ (vcpkg) ----------------
# Explicit path to avoid Qt Creator interference
set(ZeroMQ_DIR
    "D:/RP/cnh/vcpkg/installed/x64-mingw-dynamic/share/ZeroMQ"
)
find_package(ZeroMQ CONFIG REQUIRED)
# ------------------------------------------------
qt_add_qml_module(appHMITestApp
    URI HMITestApp
    VERSION 1.0
    QML_FILES
        RESOURCES qml.qrc
    SOURCES
        zmqpublisher.h
        zmqpublisher.cpp
        zmqsubscriber.h
        zmqsubscriber.cpp
)
# -------------------------------------------------------
# ZeroMQ link libraries MUST be placed AFTER target
# -------------------------------------------------------
target_link_libraries(appHMITestApp
    PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::Widgets
        libzmq        # ✅ ZeroMQ C library
)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appHMITestApp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appHMITestApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
include(GNUInstallDirs)
install(TARGETS appHMITestApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
