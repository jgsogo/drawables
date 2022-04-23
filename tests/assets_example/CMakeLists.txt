cmake_minimum_required(VERSION 3.15)

find_package(spdlog REQUIRED)

add_library(assets_example
        connection.cpp
        connection.h
        lego_part.h
        lego_track.cpp
        lego_track.h
        library.h
        parser_tracks.h
        )
set_target_properties(assets_example PROPERTIES
        PUBLIC_HEADER "connection.h;library.h;track.h"
        )
target_link_libraries(assets_example
        PUBLIC
            drawables
        PRIVATE
            parser
            spdlog::spdlog
        )

add_executable(assets_example_validator
        ${CMAKE_SOURCE_DIR}/src/validator/main.cpp
        import_static.cpp
        )

target_link_libraries(assets_example_validator
        PRIVATE
            validator-lib
            assets_example
        )
target_compile_definitions(assets_example_validator
        PRIVATE
            -DDEFAULT_FILENAME="${CMAKE_CURRENT_SOURCE_DIR}/TracksRC.xml"
        )
