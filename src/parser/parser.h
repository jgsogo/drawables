#pragma once

#include <rapidxml.hpp>

#include "drawables/drawable.h"
#include "drawables/library.h"
#include "texture_loader.h"


namespace drawables {

    class Parser {
    public:
        using NodeParsed = std::pair<std::string, std::shared_ptr<BaseDrawable>>;
        using NodeParserCallback = std::function<NodeParsed(rapidxml::xml_node<> *, const std::filesystem::path &, TextureLoader &)>;

        explicit Parser(TextureLoader &loader);

        ~Parser() = default;

        void registerNodeParser(std::string_view nodeType, const NodeParserCallback &parseFunction);

        NodeParsed parse(rapidxml::xml_node<> *node, const std::filesystem::path &filename);

        Library parse(const std::filesystem::path &filename);

    protected:
        TextureLoader &_loader;
        std::map<std::string, NodeParserCallback> _parseMethods;
    };

    class ParserRegistry {
    public:
        static ParserRegistry &instance();

        void populateParser(Parser &);

        void registerLibrary(const std::function<void(Parser &)> &);

    protected:
        ParserRegistry() = default;

        ~ParserRegistry() = default;

        std::vector<std::function<void(Parser &)>> _libraries;
    };

}
