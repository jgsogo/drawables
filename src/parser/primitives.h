#pragma once

#include <rapidxml.hpp>
#include <algorithm>

#include "primitives/line_element.hpp"
#include "primitives/arc.hpp"
#include "primitives/line.hpp"
#include "primitives/rectangle.hpp"
#include "primitives/text.hpp"
#include "primitives/polygon.hpp"
#include "primitives/image.hpp"

#include "texture_loader.h"
#include "helpers.h"

namespace drawables::parser {
    template<typename PrimitiveType>
    using PrimitivePtr = std::shared_ptr<PrimitiveType>;

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    PrimitivePtr<primitives::LineElement<Origin, TDrawList, PixelsSymbol>> parseArc(rapidxml::xml_node<> *node) {
        auto arc = std::make_shared<typename PrimitivePtr<primitives::Arc<Origin, TDrawList, PixelsSymbol>>::element_type>();
        arc->color = parseColor(node->first_attribute("color")->value());
        arc->thickness = stoXXX<math::types::NamedUnitT<float, PixelsSymbol>>(node->first_attribute("thickness")->value());
        arc->center = parsePoint<math::types::NamedUnitT<float, Origin>>(node->first_node("center"));
        arc->radius = stoXXX<math::types::NamedUnitT<float, Origin>>(node->first_node("radius")->value());
        auto angle = node->first_node("angle");
        arc->start = Magnum::Deg{std::stof(angle->first_attribute("start")->value())};
        arc->end = Magnum::Deg{std::stof(angle->first_attribute("end")->value())};
        arc->dirNormal = std::stoi(angle->first_attribute("dir")->value()) >= 0;
        return arc;
    }

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    PrimitivePtr<primitives::LineElement<Origin, TDrawList, PixelsSymbol>> parseLine(rapidxml::xml_node<> *node) {
        auto line = std::make_shared<typename PrimitivePtr<primitives::Line<Origin, TDrawList, PixelsSymbol>>::element_type>();
        line->color = parseColor(node->first_attribute("color")->value());
        line->thickness = stoXXX<math::types::NamedUnitT<float, PixelsSymbol>>(node->first_attribute("thickness")->value());
        line->start = parsePoint<math::types::NamedUnitT<float, Origin>>(node->first_node("start"));
        line->end = parsePoint<math::types::NamedUnitT<float, Origin>>(node->first_node("end"));
        return line;
    }

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    PrimitivePtr<primitives::Primitive<Origin, TDrawList, PixelsSymbol>> parseRectangle(rapidxml::xml_node<> *node) {
        auto rect = std::make_shared<typename PrimitivePtr<primitives::Rectangle<Origin, TDrawList, PixelsSymbol>>::element_type>();

        auto borderColor = node->first_attribute("borderColor");
        rect->border = bool(borderColor);
        if (rect->border) {
            rect->borderColor = parseColor(borderColor->value());
            rect->borderThickness = stoXXX<math::types::NamedUnitT<float, PixelsSymbol>>(node->first_attribute("borderThickness")->value());
        }
        auto fillColor = node->first_attribute("fillColor");
        rect->filled = bool(fillColor);
        if (rect->filled) {
            rect->fillColor = parseColor(fillColor->value());
        }

        rect->rect = parseRange<math::types::NamedUnitT<float, Origin>>(node);
        return rect;
    }

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    PrimitivePtr<primitives::Primitive<Origin, TDrawList, PixelsSymbol>> parseText(rapidxml::xml_node<> *node) {
        auto text = std::make_shared<typename PrimitivePtr<primitives::Text<Origin, TDrawList, PixelsSymbol>>::element_type>();
        auto posTranslation = parsePoint<math::types::NamedUnitT<float, Origin>>(node->first_node("position"));
        text->position.set(posTranslation);
        text->content = node->first_node("content")->value();
        text->fontSize = stoXXX<float>(node->first_attribute("font_size")->value());
        return text;
    }

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    std::vector<PrimitivePtr<primitives::LineElement<Origin, TDrawList, PixelsSymbol>>> parseLineElements(rapidxml::xml_node<> *node) {
        std::vector<std::pair<int, PrimitivePtr<primitives::LineElement<Origin, TDrawList, PixelsSymbol>> >> ret;
        for (rapidxml::xml_node<> *primitiveNode = node->first_node(); primitiveNode; primitiveNode = primitiveNode->next_sibling()) {
            auto name = primitiveNode->name();
            auto order = std::stoi(primitiveNode->first_attribute("order")->value());
            if (strcmp(name, "arc") == 0) {
                ret.emplace_back(order, parseArc<Origin, TDrawList, PixelsSymbol>(primitiveNode));
            } else if (strcmp(name, "line") == 0) {
                ret.emplace_back(order, parseLine<Origin, TDrawList, PixelsSymbol>(primitiveNode)
                );
            } else {
                //spdlog::error("Parser not implemented for primitive '{}'", name);
            }
        }

        // Get vector ordered
        std::sort(ret.begin(), ret.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.first < rhs.
                    first;
        });

        std::vector<PrimitivePtr<primitives::LineElement<Origin, TDrawList, PixelsSymbol>>> lineElements;
        std::transform(ret.begin(), ret.end(), std::back_inserter(lineElements), [](auto &item) {
            return item.second;
        });
        return lineElements;
    }

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    PrimitivePtr<primitives::Primitive<Origin, TDrawList, PixelsSymbol>> parsePolygon(rapidxml::xml_node<> *node) {
        auto polygon = std::make_shared<typename PrimitivePtr<primitives::ConvexPolygon<Origin, TDrawList, PixelsSymbol>>::element_type>();
        polygon->lineElements = parseLineElements<Origin, TDrawList, PixelsSymbol>(node);

        auto borderColor = node->first_attribute("borderColor");
        polygon->border = bool(borderColor);
        if (polygon->border) {
            polygon->borderColor = parseColor(borderColor->value());
            polygon->borderThickness = stoXXX<math::types::NamedUnitT<float, PixelsSymbol>>(node->first_attribute("borderThickness")->value());
        }
        auto fillColor = node->first_attribute("fillColor");
        polygon->filled = bool(fillColor);
        if (polygon->filled) {
            polygon->fillColor = parseColor(fillColor->value());
        }

        return polygon;
    }

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    std::vector<PrimitivePtr<primitives::Primitive<Origin, TDrawList, PixelsSymbol>>> parsePrimitives(rapidxml::xml_node<> *node) {
        std::vector<std::pair<int, PrimitivePtr<primitives::Primitive<Origin, TDrawList, PixelsSymbol>>>> ret;
        for (rapidxml::xml_node<> *primitiveNode = node->first_node(); primitiveNode; primitiveNode = primitiveNode->next_sibling()) {
            auto name = primitiveNode->name();
            auto order = std::stoi(primitiveNode->first_attribute("order")->value());
            if (strcmp(name, "arc") == 0) {
                ret.emplace_back(order, parseArc<Origin, TDrawList, PixelsSymbol>(primitiveNode));
            } else if (strcmp(name, "line") == 0) {
                ret.emplace_back(order, parseLine<Origin, TDrawList, PixelsSymbol>(primitiveNode));
            } else if (strcmp(name, "polygon") == 0) {
                ret.emplace_back(order, parsePolygon<Origin, TDrawList, PixelsSymbol>(primitiveNode));
            } else if (strcmp(name, "rectangle") == 0) {
                ret.emplace_back(order, parseRectangle<Origin, TDrawList, PixelsSymbol>(primitiveNode));
            } else if (strcmp(name, "text") == 0) {
                ret.emplace_back(order, parseText<Origin, TDrawList, PixelsSymbol>(primitiveNode)
                );
            } else {
                //spdlog::error("Parser not implemented for primitive '{}'", name);
            }
        }

        // Get vector ordered
        std::sort(ret.begin(), ret.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.first < rhs.first;
        });

        std::vector<PrimitivePtr<primitives::Primitive<Origin, TDrawList, PixelsSymbol>>> primitives;
        std::transform(ret.begin(), ret.end(), std::back_inserter(primitives), [](auto &item) {
            return item.second;
        });
        return primitives;
    }

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    PrimitivePtr<primitives::Image<Origin, TDrawList, PixelsSymbol>> parseImagefile(rapidxml::xml_node<> *node,
                                                                                    const std::filesystem::path &filename,
                                                                                    drawables::TextureLoader &loader) {
        auto imageFilename = std::filesystem::path{node->first_node("filename")->value()};
        if (imageFilename.is_relative()) {
            imageFilename = filename.parent_path() / imageFilename;
        }
        int width, height;
        auto &texture = loader.loadTexture(imageFilename, width, height);

        auto imagefile = std::make_shared<typename PrimitivePtr<primitives::Image<Origin, TDrawList, PixelsSymbol>>::element_type>(texture);
        imagefile->texture.uvCoordinates = parseUVRange<render::UVCoordinates>(node->first_node("uv_coordinates"), width, height);
        return imagefile;
    }

}
