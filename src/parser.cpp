#include "parser.h"

namespace drawables {

    Parser::Parser(TextureLoader &loader) : _loader{loader} {};

    void Parser::registerNodeParser(std::string_view nodeType, const NodeParserCallback& parseFunction) {
        _parseMethods.insert(std::make_pair(nodeType, parseFunction));
    }

    std::pair<std::string, std::shared_ptr<BaseDrawable>> Parser::parse(rapidxml::xml_node<> *node,
                                                                        const std::filesystem::path &filename) {
        auto type = node->first_attribute("type");
        auto it = _parseMethods.find(type->value());
        if (it != _parseMethods.end()) {
            return it->second(node, filename, _loader);
        } else {
            // TODO: Handle error
        }

    }
}
