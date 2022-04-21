cmake_minimum_required(VERSION 3.15)

add_subdirectory(assets_example)

find_package(Catch2 REQUIRED)

enable_testing()

add_executable(unit_tests
        main.cpp
        test_drawables/test_drawable.cpp
        )
target_link_libraries(unit_tests
        Catch2::Catch2
        ${PROJECT_NAME}
        )

# Load and use the .cmake file provided by Catch so all the test cases
# are made available to CTest.
include(Catch)
catch_discover_tests(unit_tests)
