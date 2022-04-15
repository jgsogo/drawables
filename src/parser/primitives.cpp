#include "primitives.h"

#include <algorithm>
#include <spdlog/spdlog.h>

#include "helpers.h"

namespace drawables::parser {

    PrimitivePtr<primitives::LineElement> parseArc(rapidxml::xml_node<> *node) {
        auto arc = std::make_shared<PrimitivePtr<primitives::Arc>::element_type>();
        arc->color = parseColor(node->first_attribute("color")->value());
        arc->thickness = stoXXX<::render::imgui::Pixels>(node->first_attribute("thickness")->value());
        arc->center = parsePoint<::math::Milimeters>(node->first_node("center"));
        arc->radius = stoXXX<::math::Milimeters>(node->first_node("radius")->value());
        auto angle = node->first_node("angle");
        arc->start = Magnum::Deg{std::stof(angle->first_attribute("start")->value())};
        arc->end = Magnum::Deg{std::stof(angle->first_attribute("end")->value())};
        arc->dirNormal = std::stoi(angle->first_attribute("dir")->value()) >= 0;
        return arc;
    }

    PrimitivePtr<primitives::LineElement> parseLine(rapidxml::xml_node<> *node) {
        auto line = std::make_shared<PrimitivePtr<primitives::Line>::element_type>();
        line->color = parseColor(node->first_attribute("color")->value());
        line->thickness = stoXXX<render::imgui::Pixels>(node->first_attribute("thickness")->value());
        line->start = parsePoint<::math::Milimeters>(node->first_node("start"));
        line->end = parsePoint<::math::Milimeters>(node->first_node("end"));
        return line;
    }

    PrimitivePtr<primitives::Primitive> parseRectangle(rapidxml::xml_node<> *node) {
        auto rect = std::make_shared<PrimitivePtr<primitives::Rectangle>::element_type>();

        auto borderColor = node->first_attribute("borderColor");
        rect->border = bool(borderColor);
        if (rect->border) {
            rect->borderColor = parseColor(borderColor->value());
            rect->borderThickness = stoXXX<render::imgui::Pixels>(node->first_attribute("borderThickness")->value());
        }
        auto fillColor = node->first_attribute("fillColor");
        rect->filled = bool(fillColor);
        if (rect->filled) {
            rect->fillColor = parseColor(fillColor->value());
        }

        rect->rect = parseRange<::math::Milimeters>(node);
        return rect;
    }

    PrimitivePtr<primitives::Primitive> parseText(rapidxml::xml_node<> *node) {
        auto text = std::make_shared<PrimitivePtr<primitives::Text>::element_type>();
        auto posTranslation = parsePoint<math::Milimeters>(node->first_node("position"));
        text->position.set(posTranslation);
        text->content = node->first_node("content")->value();
        text->fontSize = stoXXX<float>(node->first_attribute("font_size")->value());
        return text;
    }

    PrimitivePtr<primitives::Primitive> parsePolygon(rapidxml::xml_node<> *node) {
        auto polygon = std::make_shared<PrimitivePtr<primitives::ConvexPolygon>::element_type>();
        polygon->lineElements = parseLineElements(node);

        auto borderColor = node->first_attribute("borderColor");
        polygon->border = bool(borderColor);
        if (polygon->border) {
            polygon->borderColor = parseColor(borderColor->value());
            polygon->borderThickness = stoXXX<render::imgui::Pixels>(node->first_attribute("borderThickness")->value());
        }
        auto fillColor = node->first_attribute("fillColor");
        polygon->filled = bool(fillColor);
        if (polygon->filled) {
            polygon->fillColor = parseColor(fillColor->value());
        }

        return polygon;
    }

    std::vector<PrimitivePtr<primitives::LineElement>> parseLineElements(rapidxml::xml_node<> *node) {
        std::vector<std::pair<int, PrimitivePtr<primitives::LineElement> >> ret;
        for (rapidxml::xml_node<> *primitiveNode = node->first_node(); primitiveNode; primitiveNode = primitiveNode->next_sibling()) {
            auto name = primitiveNode->name();
            auto order = std::stoi(primitiveNode->first_attribute("order")->value());
            if (strcmp(name, "arc") == 0) {
                ret.emplace_back(order, parseArc(primitiveNode));
            } else if (strcmp(name, "line") == 0) {
                ret.emplace_back(order, parseLine(primitiveNode)
                );
            } else {
                spdlog::error("Parser not implemented for primitive '{}'", name);
            }
        }

        // Get vector ordered
        std::sort(ret.begin(), ret.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.first < rhs.
                    first;
        });

        std::vector<PrimitivePtr<primitives::LineElement>> lineElements;
        std::transform(ret.begin(), ret.end(), std::back_inserter(lineElements), [](auto &item) {
            return item.second;
        });
        return lineElements;
    }

    std::vector<PrimitivePtr<primitives::Primitive>> parsePrimitives(rapidxml::xml_node<> *node) {
        std::vector<std::pair<int, PrimitivePtr<primitives::Primitive>>> ret;
        for (rapidxml::xml_node<> *primitiveNode = node->first_node(); primitiveNode; primitiveNode = primitiveNode->next_sibling()) {
            auto name = primitiveNode->name();
            auto order = std::stoi(primitiveNode->first_attribute("order")->value());
            if (strcmp(name, "arc") == 0) {
                ret.emplace_back(order, parseArc(primitiveNode));
            } else if (strcmp(name, "line") == 0) {
                ret.emplace_back(order, parseLine(primitiveNode));
            } else if (strcmp(name, "polygon") == 0) {
                ret.emplace_back(order, parsePolygon(primitiveNode));
            } else if (strcmp(name, "rectangle") == 0) {
                ret.emplace_back(order, parseRectangle(primitiveNode));
            } else if (strcmp(name, "text") == 0) {
                ret.emplace_back(order, parseText(primitiveNode)
                );
            } else {
                spdlog::error("Parser not implemented for primitive '{}'", name);
            }
        }

        // Get vector ordered
        std::sort(ret.begin(), ret.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.first < rhs.first;
        });

        std::vector<PrimitivePtr<primitives::Primitive>> primitives;
        std::transform(ret.begin(), ret.end(), std::back_inserter(primitives), [](auto &item) {
            return item.second;
        });
        return primitives;
    }

    PrimitivePtr<primitives::Image> parseImagefile(rapidxml::xml_node<> *node, const std::filesystem::path &filename, drawables::TextureLoader &loader) {
        auto imageFilename = std::filesystem::path{node->first_node("filename")->value()};
        if (imageFilename.is_relative()) {
            imageFilename = filename.parent_path() / imageFilename;
        }
        int width, height;
        auto &texture = loader.loadTexture(imageFilename, width, height);

        auto imagefile = std::make_shared<PrimitivePtr<primitives::Image>::element_type>(texture);
        imagefile->texture.uvCoordinates = parseUVRange<render::UVCoordinates>(node->first_node("uv_coordinates"), width, height);
        return imagefile;
    }

}
