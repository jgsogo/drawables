
find_package(MagnumPlugins REQUIRED PngImporter)

add_library(validator-lib INTERFACE)
target_link_libraries(validator-lib
        INTERFACE
            parser
            spdlog::spdlog
            Magnum::Sdl2Application
            MagnumIntegration::ImGui
            MagnumPlugins::PngImporter
        )

add_executable(validator
        main.cpp
        )
target_link_libraries(validator
        PRIVATE
        validator-lib
        )
