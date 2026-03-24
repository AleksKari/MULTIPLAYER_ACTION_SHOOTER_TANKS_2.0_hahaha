cmake_minimum_required(VERSION 3.16)

project(SUPER_MEGA_TANKS)

set(CMAKE_CXX_STANDART 17)

include_directories(include)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(SUPER_MEGA_TANKS src/window.cpp)
target_link_libraries(SUPER_MEGA_TANKS sfml-graphics sfml-window sfml-system)