#pragma once

#include <rapidxml.hpp>
#include <memory>  // TODO: remove after https://github.com/jgsogo/render_context/issues/40

#include "primitives/line_element.hpp"
#include "primitives/arc.hpp"
#include "primitives/line.hpp"
#include "primitives/rectangle.hpp"
#include "primitives/text.hpp"
#include "primitives/polygon.hpp"
#include "primitives/image.hpp"

#include "texture_loader.h"

namespace drawables::parser {
    using namespace math::units;
    using namespace render::imgui::units;

    template<template<const char *, typename, const char *> typename PrimitiveType>
    using PrimitivePtr = std::shared_ptr<PrimitiveType<math::units::mm, ImDrawList, render::imgui::units::impx>>;

    PrimitivePtr<primitives::LineElement> parseArc(rapidxml::xml_node<> *node);

    PrimitivePtr<primitives::LineElement> parseLine(rapidxml::xml_node<> *node);

    PrimitivePtr<primitives::Primitive> parseRectangle(rapidxml::xml_node<> *node);

    PrimitivePtr<primitives::Primitive> parseText(rapidxml::xml_node<> *node);

    PrimitivePtr<primitives::Primitive> parsePolygon(rapidxml::xml_node<> *node);

    std::vector<PrimitivePtr<primitives::LineElement>> parseLineElements(rapidxml::xml_node<> *node);

    std::vector<PrimitivePtr<primitives::Primitive>> parsePrimitives(rapidxml::xml_node<> *node);

    PrimitivePtr<primitives::Image> parseImagefile(rapidxml::xml_node<> *node,
                                                   const std::filesystem::path &filename,
                                                   drawables::TextureLoader &loader);

}
