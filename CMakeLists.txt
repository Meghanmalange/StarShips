cmake_minimum_required(VERSION 3.20)

project(RocketSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(RocketSimulator
    src/main.cpp
)

target_link_libraries(RocketSimulator PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)