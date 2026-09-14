cmake_minimum_required(VERSION 3.20)

project(RocketSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(RocketSimulator
    src/main.cpp
    src/ui/mainMenu.cpp
    src/ui/rocketConfig.cpp
    src/simulation/camera.cpp
    src/simulation/inputManager.cpp
    src/simulation/physicsEngine.cpp
    src/simulation/render.cpp
    src/simulation/simulation.cpp
)

target_include_directories(RocketSimulator PRIVATE
    include
)

target_link_libraries(RocketSimulator PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)