cmake_minimum_required(VERSION 3.15)

add_library(drawables INTERFACE)
set_target_properties(drawables PROPERTIES
        PUBLIC_HEADER "connection.h;drawable.h;id.h;library.h"
        )
target_include_directories(drawables INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(drawables
        INTERFACE
            render_context::render_context
        )

install(TARGETS drawables
        PUBLIC_HEADER DESTINATION include/drawables
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
