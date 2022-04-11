
find_package(MagnumPlugins REQUIRED PngImporter)

add_executable(validator
        main.cpp
        )

target_link_libraries(validator
        PRIVATE
        parser
        spdlog::spdlog
        Magnum::Sdl2Application
        MagnumIntegration::ImGui
        MagnumPlugins::PngImporter
        )
