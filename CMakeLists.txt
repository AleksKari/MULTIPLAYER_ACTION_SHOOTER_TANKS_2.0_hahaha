cmake_minimum_required(VERSION 3.16)
project(project CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к исходникам
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/libraries")

# Рекурсивно собираем все .cpp
file(GLOB_RECURSE SOURCES
    "${SRC_DIR}/*.cpp"
    "${LIB_DIR}/*.cpp"
)

# Добавляем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Добавляем include директории
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_DIR}
    ${LIB_DIR}
)

# SFML
find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)

target_link_libraries(${PROJECT_NAME}
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    sfml-network
)

# Копируем textures рядом с бинарником
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/textures"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/textures"
)

