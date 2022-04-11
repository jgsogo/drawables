cmake_minimum_required(VERSION 3.15)

find_package(render_context REQUIRED)

add_library(${PROJECT_NAME}
        connection.h
        drawable.cpp
        drawable.h
        id.h
        )
set_target_properties(${PROJECT_NAME} PROPERTIES
        PUBLIC_HEADER "connection.h;drawable.h;id.h"
        )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC render_context::render_context)

install(TARGETS ${PROJECT_NAME}
        PUBLIC_HEADER DESTINATION include
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
