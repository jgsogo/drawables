cmake_minimum_required(VERSION 3.15)

find_package(spdlog REQUIRED)
find_package(rapidxml REQUIRED)
find_package(fmt REQUIRED)
find_package(render_context REQUIRED)

add_subdirectory(drawables)
add_subdirectory(parser)

option(BUILD_TOOLS "Also build the tools" ON)
if (BUILD_TOOLS)
    add_subdirectory(validator)
endif()
