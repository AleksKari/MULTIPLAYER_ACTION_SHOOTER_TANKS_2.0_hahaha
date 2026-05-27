find_package(GTest REQUIRED)

add_executable(game_tests
    test_vec2.cpp
    test_collision.cpp
    test_projectile.cpp
    test_player.cpp
)

target_link_libraries(game_tests PRIVATE game_lib GTest::gtest GTest::gtest_main)
target_include_directories(game_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libraries
)

target_compile_options(game_tests PRIVATE --coverage)
target_link_options(game_tests PRIVATE --coverage)
target_compile_options(game_lib PRIVATE --coverage)
target_link_options(game_lib PRIVATE --coverage)

include(GoogleTest)
gtest_discover_tests(game_tests)
