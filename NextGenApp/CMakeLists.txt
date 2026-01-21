cmake_minimum_required(VERSION 3.16)

project(NextGenApp VERSION 0.1 LANGUAGES CXX)

#  set orientation to [ PORTRAIT ] or [ LANDSCAPE ]
set(ORIENTATION "PORTRAIT")


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

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

set(PROJECT_SOURCES
        main.cpp
        ./qml/qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NextGenApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ./qml/images.qrc
        ./qml/Fonts.qrc
        include/commonlib_global.h include/clogger.h
        src/clogger.cpp
        include/appinterface.h src/appinterface.cpp
        include/constants.h
    )
target_compile_definitions(NextGenApp PRIVATE ORIENTATION="${ORIENTATION}")

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET NextGenApp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(NextGenApp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(NextGenApp
          ${PROJECT_SOURCES}
        )
    endif()
endif()

# -------------------------------------------------------
# ZeroMQ include + link libraries MUST be placed AFTER target
# -------------------------------------------------------
if(WIN32)
    target_link_libraries(NextGenApp PRIVATE libzmq)
else()
    target_include_directories(NextGenApp PRIVATE ${ZMQ_INCLUDE_DIRS})
    target_link_libraries(NextGenApp PRIVATE ${ZMQ_LIBRARIES})
endif()

target_link_libraries(NextGenApp
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.NextGenApp)
endif()
set_target_properties(NextGenApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS NextGenApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(NextGenApp)
    qt_finalize_executable(NextGenApp)
endif()
