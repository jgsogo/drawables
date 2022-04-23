#pragma once

#include <Magnum/ImGuiIntegration/Context.hpp>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Platform/Sdl2Application.h>

#include "parser/texture_loader.h"

class TheApplication : public Magnum::Platform::Application {
public:
    explicit TheApplication(Magnum::Platform::Application::Arguments &args) : Magnum::Platform::Application(args) {}

    void drawEvent() override {}

public:
    drawables::TextureLoader loader;
};