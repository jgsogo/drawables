cmake_minimum_required(VERSION 3.15)

add_library(parser
        enums.hpp
        helpers.cpp
        helpers.h
        parser.cpp
        parser.h
        primitives.cpp
        primitives.h
        texture_loader.cpp
        texture_loader.h
        )
set_target_properties(parser PROPERTIES
        PUBLIC_HEADER "enums.hpp;helpers.h;parser.h;primitives.h;texture_loader.h"
        )
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(parser
        PUBLIC
            drawables
            rapidxml::rapidxml
        PRIVATE
            spdlog::spdlog
        )

install(TARGETS parser
        PUBLIC_HEADER DESTINATION include/parser
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
