cmake_minimum_required(VERSION 3.16)
project(project CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/libraries")
set(EXT_DIR "${CMAKE_SOURCE_DIR}/external")

file(GLOB_RECURSE SOURCES
    "${SRC_DIR}/*.cpp"
    "${LIB_DIR}/*.cpp"
    "${EXT_DIR}/*.hpp"
)
list(FILTER SOURCES EXCLUDE REGEX ".*Window\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX ".*server\\.cpp$")

add_executable(${PROJECT_NAME} ${SOURCES} ${SRC_DIR}/Window.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_DIR}
    ${LIB_DIR}
    ${CMAKE_SOURCE_DIR}/external
)

find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

target_link_libraries(${PROJECT_NAME}
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    sfml-network
)

add_executable(server ${SRC_DIR}/server.cpp)
target_include_directories(server PRIVATE ${SRC_DIR} ${LIB_DIR} ${EXT_DIR})
target_link_libraries(server sfml-system sfml-network)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/textures"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/textures"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/sounds"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/sounds"
)

add_library(game_lib STATIC ${SOURCES})
target_include_directories(game_lib PUBLIC ${SRC_DIR} ${LIB_DIR} ${EXT_DIR})
target_link_libraries(game_lib PUBLIC sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

enable_testing()
add_subdirectory(tests)
