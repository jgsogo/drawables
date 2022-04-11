#include "parser_tracks.h"

#include <spdlog/spdlog.h>
#include "parser/enums.hpp"
#include "parser/helpers.hpp"
#include "parser/primitives.h"

#include "lego_part.h"
#include "lego_track.h"


namespace example::parser {

    template<typename PartType>
    std::pair<std::string, std::shared_ptr<PartType>> _parsePart(rapidxml::xml_node<> *node,
                                                                 const std::filesystem::path &filename,
                                                                 drawables::TextureLoader &loader) {
        std::string id = node->first_attribute("id")->value();
        auto p = std::make_shared<PartType>();

        p->name = node->first_node("name")->value();
        p->description = drawables::parser::optionalStr(node->first_node("description"));
        p->bbox = drawables::parser::parseRange<::math::Milimeters>(node->first_node("bbox"));

        auto connections = node->first_node("connections");
        for (rapidxml::xml_node<> *conNode = connections->first_node("connection"); conNode; conNode = conNode->next_sibling()) {
            example::LegoTrack::Connection con;
            auto posTranslation = drawables::parser::parsePoint<::math::Milimeters>(conNode->first_node("position"));
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
                p->renderData[lod] = drawables::parser::parsePrimitives(renderNode);
            } else if (strcmp(type, "imagefile") == 0) {
                auto imagefile = drawables::parser::parseImagefile(renderNode, filename, loader);
                imagefile->bbox = p->bbox;
                p->renderData[lod] = {imagefile};
            } else {
                spdlog::error("Unknown render node with type '{}'", type);
            }
        }

        return std::make_pair(id, p);
    }

    drawables::Parser::NodeParsed parseLegoPart(rapidxml::xml_node<> *node,
                                                const std::filesystem::path &filename,
                                                drawables::TextureLoader &loader) {
        spdlog::debug("Parse example::LegoPart");
        return _parsePart<LegoPart>(node, filename, loader);
    }

    drawables::Parser::NodeParsed parseLegoTrack(rapidxml::xml_node<> *node,
                                                 const std::filesystem::path &filename,
                                                 drawables::TextureLoader &loader) {
        spdlog::debug("Parse example::LegoTrack");
        auto[id, part] = _parsePart<LegoTrack>(node, filename, loader);
        part->volt = drawables::parser::parseEnum<example::Voltage>(node->first_node("voltage")->value());
        part->gauge = drawables::parser::parseEnum<example::Gauge>(node->first_node("gauge")->value());
        return std::make_pair(id, part);
    }
}
