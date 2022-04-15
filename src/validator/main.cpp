
#include <iostream>
#include <Magnum/ImGuiIntegration/Context.hpp>
#include <Magnum/GL/Renderer.h>
#include <spdlog/spdlog.h>
#include <Magnum/Platform/Sdl2Application.h>

#include "parser/parser.h"

class TheApplication : public Magnum::Platform::Application {
public:
    explicit TheApplication(Magnum::Platform::Application::Arguments &args) : Magnum::Platform::Application(args) {}

    void drawEvent() override {}

public:
    drawables::TextureLoader loader;
};

#ifndef DEFAULT_FILENAME
#define DEFAULT_FILENAME "";
#endif

int main(int argc, char **argv) {
    std::string library_filename = DEFAULT_FILENAME;

    // Retrieve the (non-option) argument:
    if ((argc <= 1) || (argv[argc - 1] == nullptr) || (argv[argc - 1][0] == '-')) {  // there is NO input...
        if (library_filename.empty()) {
            std::cerr << "No argument provided!" << std::endl;
            return 1;
        }
    } else {  // there is an input...
        library_filename = argv[argc - 1];
    }

    // Because we are going to load textures, we need the GL context
    auto args = Magnum::Platform::Application::Arguments{argc, argv};
    auto app = TheApplication{args};

    spdlog::set_level(spdlog::level::trace);
    spdlog::info("Parse file '{}'", library_filename);
    drawables::Parser parser(app.loader);
    drawables::ParserRegistry::instance().populateParser(parser);
    auto library = parser.parse(library_filename);

    std::cout << "Library: " << library.getName() << std::endl;
    for (auto&[id, part]: library.getDrawables()) {
        std::cout << "  - Part: " << id << std::endl;
        std::cout << "\t" << part->name << std::endl;
        std::cout << "\t" << part->description.value_or("<no-description>") << std::endl;
    }
    spdlog::info("Done with the library '{}'", library.getName());
    spdlog::shutdown();
}
