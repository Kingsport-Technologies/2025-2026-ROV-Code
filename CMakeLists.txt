cmake_minimum_required(VERSION 3.10)
project(RobotCode2526 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)

FetchContent_Declare(
    websocketpp
    GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git
    GIT_TAG        0.8.2
)

set(CMAKE_POLICY_VERSION_MINIMUM 3.10)
FetchContent_MakeAvailable(nlohmann_json websocketpp)
unset(CMAKE_POLICY_VERSION_MINIMUM)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-30-2
)
FetchContent_MakeAvailable(asio)

add_executable(topend
    src/topcomputer.cpp
    src/pwmmath.cpp
    src/outgoingserver.cpp
    src/websocketserver.cpp
)
target_compile_definitions(topend PRIVATE
    ASIO_STANDALONE
    _WEBSOCKETPP_CPP11_STL_
)
target_include_directories(topend PRIVATE
    include/
    ${websocketpp_SOURCE_DIR}
    ${asio_SOURCE_DIR}/asio/include
)
target_link_libraries(topend PRIVATE
    nlohmann_json::nlohmann_json
)

add_executable(dummy_load
    src/dummy_load.cpp
)