cmake_minimum_required(VERSION 3.16)
project(server-status)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXPORT_COMPILE_COMMANDS ON)

find_package(cpr REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(server-status
    src/main.cpp
)

target_include_directories(server-status PRIVATE src)

target_link_libraries(server-status PRIVATE nlohmann_json::nlohmann_json cpr::cpr)
