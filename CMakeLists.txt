cmake_minimum_required(VERSION 3.10)
project(RobotCode2526 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_TARGET "TOPEND" CACHE STRING "Which end to build: TOPEND or BOTTOMEND")
if(BUILD_TARGET STREQUAL "TOPEND")
    set(CMAKE_AUTOMOC ON)
    find_package(Qt6 COMPONENTS Core Quick Widgets Multimedia MultimediaWidgets REQUIRED)
    find_package(SDL3 REQUIRED)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(GST REQUIRED
        gstreamer-1.0
        gstreamer-video-1.0
        gstreamer-app-1.0
        gstreamer-base-1.0
    )

    qt_add_resources(SOURCES resources/resources.qrc)

    add_executable(topend
        ${SOURCES}
        src/topcomputer.cpp
        src/pwmmath.cpp
        src/outgoingserver.cpp
        src/settingsDialog.cpp
        src/pilot_window.cpp
        src/controller_interface.cpp
        src/pwmmath.cpp
        include/pilot_window.hpp
        include/settingsDialog.hpp
        include/controller_interface.hpp
        include/pwmmath.hpp
        include/outgoingserver.hpp
        include/json.hpp
    )

    target_include_directories(topend PRIVATE
        include/
        ${GST_INCLUDE_DIRS}
    )
    target_link_libraries(topend PRIVATE
        Qt6::Core Qt6::Quick Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets
        SDL3::SDL3
        ${GST_LIBRARIES}
    )

    add_executable(dummy_load
        src/dummy_load.cpp
    )
elseif(BUILD_TARGET STREQUAL "BOTTOMEND")
    # Check that pigpio is installed (built from source)
    find_library(PIGPIO_LIB pigpio REQUIRED)
    find_program(PIGPIOD_EXECUTABLE pigpiod REQUIRED)

    if(NOT PIGPIO_LIB)
        message(FATAL_ERROR "libpigpio not found. Build and install from source: https://github.com/joan2937/pigpio")
    endif()

    if(NOT PIGPIOD_EXECUTABLE)
        message(FATAL_ERROR "pigpiod not found. Build and install from source: https://github.com/joan2937/pigpio")
    endif()

    message(STATUS "Found pigpio: ${PIGPIO_LIB}")
    message(STATUS "Found pigpiod: ${PIGPIOD_EXECUTABLE}")
    execute_process(
        COMMAND systemctl is-active pigpiod
        OUTPUT_VARIABLE PIGPIOD_STATUS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(NOT PIGPIOD_STATUS STREQUAL "active")
        message(WARNING "pigpiod is not currently running. Start it with: sudo systemctl start pigpiod")
    endif()
    add_executable(bottomend
        src/bottomed.cpp
        include/json.hpp
    )
    target_link_libraries(bottomend
        ${PIGPIO_LIB}
        
    )
else()
    message(FATAL_ERROR "BUILD_TARGET must be TOPEND or BOTTOMEND, got: ${BUILD_TARGET}")
endif()