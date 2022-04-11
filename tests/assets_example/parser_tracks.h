#pragma once

#include "parser/parser.h"

namespace example::parser {

    drawables::Parser::NodeParsed parseLegoPart(rapidxml::xml_node<> *, const std::filesystem::path&, drawables::TextureLoader &);
    drawables::Parser::NodeParsed parseLegoTrack(rapidxml::xml_node<> *, const std::filesystem::path&, drawables::TextureLoader &);

}
