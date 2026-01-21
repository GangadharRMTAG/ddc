cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(HMITestApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appHMITestApp
    main.cpp

)
# ZeroMQ
if(WIN32)
    # Windows (vcpkg)
    set(ZeroMQ_DIR "D:/github/vcpkg/installed/x64-mingw-dynamic/share/ZeroMQ")
    find_package(ZeroMQ CONFIG REQUIRED)
else()
    # Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ZMQ REQUIRED libzmq)
endif()


qt_add_qml_module(appHMITestApp
    URI HMITestApp
    VERSION 1.0
    QML_FILES

        SOURCES zmqpublisher.h zmqpublisher.cpp zmqsubscriber.cpp zmqsubscriber.h
        RESOURCES qml.qrc
        SOURCES
)


# -------------------------------------------------------
# ZeroMQ include + link libraries MUST be placed AFTER target
# -------------------------------------------------------
if(WIN32)
    target_link_libraries(appHMITestApp PRIVATE libzmq)
else()
    target_include_directories(appHMITestApp PRIVATE ${ZMQ_INCLUDE_DIRS})
    target_link_libraries(appHMITestApp PRIVATE ${ZMQ_LIBRARIES})
endif()

target_link_libraries(appHMITestApp
    PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
)

target_link_libraries(appHMITestApp PRIVATE Qt6::Core)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appHMITestApp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appHMITestApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appHMITestApp
    PRIVATE Qt6::Quick
)
target_link_libraries(appHMITestApp PRIVATE Qt6::Core Qt6::Widgets)




include(GNUInstallDirs)
install(TARGETS appHMITestApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
