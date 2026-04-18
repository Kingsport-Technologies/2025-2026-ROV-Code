cmake_minimum_required(VERSION 3.10)
project(RobotCode2526 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


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