#pragma once

#include "parser/parser.h"
#include "parser/enums.hpp"
#include "parser/helpers.h"
#include "parser/primitives.h"

#include "lego_part.h"
#include "lego_track.h"

namespace example::parser {

    namespace _detail {
        // template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
        template<template<const char *, typename, const char *> typename TPartType, const char *Origin, typename TDrawList, const char *PixelsSymbol>
        std::pair<std::string, std::shared_ptr<TPartType<Origin, TDrawList, PixelsSymbol>>> _parsePart(rapidxml::xml_node<> *node,
                                                                                                       const std::filesystem::path &filename,
                                                                                                       drawables::TextureLoader &loader) {
            using PartType = TPartType<Origin, TDrawList, PixelsSymbol>;

            std::string id = node->first_attribute("id")->value();
            auto p = std::make_shared<PartType>();

            p->name = node->first_node("name")->value();
            p->description = drawables::parser::optionalStr(node->first_node("description"));
            p->bbox = drawables::parser::parseRange<math::types::NamedUnitT<float, Origin>>(node->first_node("bbox"));

            auto connections = node->first_node("connections");
            for (rapidxml::xml_node<> *conNode = connections->first_node("connection"); conNode; conNode = conNode->next_sibling()) {
                typename example::LegoPart<Origin, TDrawList, PixelsSymbol>::Connection con;
                auto posTranslation = drawables::parser::parsePoint<math::types::NamedUnitT<float, Origin>>(conNode->first_node("position"));
                con.position.set(posTranslation);
                con.position.set(drawables::parser::stoXXX<Magnum::Deg>(conNode->first_node("angle")->value()));
                con.type = drawables::parser::parseEnum<example::ConnectionType>(conNode->first_node("type")->value());
                p->connections.emplace_back(con);
            }

            // TODO: p.bricklink
            // TODO: p.ldraw

            // Parse render LODs
            for (rapidxml::xml_node<> *renderNode = node->first_node("render"); renderNode; renderNode = renderNode->next_sibling()) {
                auto type = renderNode->first_attribute("type")->value();
                auto lod = drawables::parser::stoXXX<int>(renderNode->first_attribute("lod")->value());
                if (strcmp(type, "primitives") == 0) {
                    p->renderData[lod] = drawables::parser::parsePrimitives<Origin, TDrawList, PixelsSymbol>(renderNode);
                } else if (strcmp(type, "imagefile") == 0) {
                    auto imagefile = drawables::parser::parseImagefile<Origin, TDrawList, PixelsSymbol>(renderNode, filename, loader);
                    imagefile->bbox = p->bbox;
                    p->renderData[lod] = {imagefile};
                } else {
                    //spdlog::error("Unknown render node with type '{}'", type);
                }
            }

            return std::make_pair(id, p);
        }
    }

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    typename drawables::Parser<Origin, TDrawList, PixelsSymbol>::NodeParsed
    parseLegoPart(rapidxml::xml_node<> *node, const std::filesystem::path &filename, drawables::TextureLoader &loader) {
        //spdlog::debug("Parse example::LegoPart");
        return _detail::_parsePart<LegoPart, Origin, TDrawList, PixelsSymbol>(node, filename, loader);
    }

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    typename drawables::Parser<Origin, TDrawList, PixelsSymbol>::NodeParsed
    parseLegoTrack(rapidxml::xml_node<> *node, const std::filesystem::path &filename, drawables::TextureLoader &loader) {
        //spdlog::debug("Parse example::LegoTrack");
        auto[id, part] = _detail::_parsePart<LegoTrack, Origin, TDrawList, PixelsSymbol>(node, filename, loader);
        part->volt = drawables::parser::parseEnum<example::Voltage>(node->first_node("voltage")->value());
        part->gauge = drawables::parser::parseEnum<example::Gauge>(node->first_node("gauge")->value());
        return std::make_pair(id, part);
    }

}
