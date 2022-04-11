#pragma once

#include "rapidxml.hpp"

#include "drawable.h"
#include "texture_loader.h"


namespace drawables {

    class Parser {
    public:
        using NodeParsed = std::pair<std::string, std::shared_ptr<BaseDrawable>>;
        using NodeParserCallback = std::function<NodeParsed (rapidxml::xml_node<> *, const std::filesystem::path&, TextureLoader &)>;

        explicit Parser(TextureLoader &loader);

        ~Parser() = default;

        void registerNodeParser(std::string_view nodeType, const NodeParserCallback& parseFunction);

        NodeParsed parse(rapidxml::xml_node<> *node, const std::filesystem::path &filename);

    protected:
        TextureLoader &_loader;
        std::map<std::string, NodeParserCallback> _parseMethods;
    };

}
