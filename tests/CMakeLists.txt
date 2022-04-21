cmake_minimum_required(VERSION 3.15)

add_subdirectory(assets_example)

find_package(Catch2 REQUIRED)
find_package(MagnumPlugins REQUIRED PngImporter)
find_package(spdlog REQUIRED)

enable_testing()

add_executable(unit_tests
        main.cpp
        assets_example/import_static.cpp
        test_drawables/test_drawable.cpp
        )
target_link_libraries(unit_tests
        parser
        assets_example
        spdlog::spdlog
        Catch2::Catch2
        Magnum::Sdl2Application
        #MagnumIntegration::ImGui
        MagnumPlugins::PngImporter
        )
target_compile_definitions(unit_tests
        PRIVATE
        -DASSETS_EXAMPLE_FILENAME="${CMAKE_CURRENT_SOURCE_DIR}/assets_example/TracksRC.xml"
        )

# Load and use the .cmake file provided by Catch so all the test cases
# are made available to CTest.
include(Catch)
catch_discover_tests(unit_tests)
