#include "parser.h"

#include <fstream>
#include <spdlog/spdlog.h>

namespace drawables {

    Parser::Parser(TextureLoader &loader) : _loader{loader} {};

    void Parser::registerNodeParser(std::string_view nodeType, const NodeParserCallback &parseFunction) {
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

    Library Parser::parse(std::string_view name, const std::filesystem::path &filename) {
        spdlog::info("Library::parse(filename='{}')", filename.string());

        // Parse the document
        rapidxml::xml_document<> doc;
        std::ifstream theFile{filename};
        std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)),
                                 std::istreambuf_iterator<char>());
        buffer.push_back('\0');

        // Parse the buffer using the xml file parsing library into doc
        doc.parse<rapidxml::parse_default>(&buffer[0]);

        // It is a part
        rapidxml::xml_node<> *root_node = doc.first_node("parts_library");
        if (root_node) {
            auto library_name = root_node->first_attribute("name")->value();
            spdlog::info(" - library parts name: {}", library_name);

            Library ret{library_name};
            for (rapidxml::xml_node<> *partNode = root_node->first_node("part"); partNode; partNode = partNode->next_sibling()) {
                auto[id, part] = this->parse(partNode, filename);
                part->id = Id{std::string{name}, id};
                ret._drawables.insert(std::make_pair(id, part));  // TODO: What if I apply XSLT transformation first instead of passing the arg?
            }
            return ret;
        } else {
            // TODO: Handle error
            spdlog::error("Cannot find root node 'parts_library' in filename '{}'", filename.string());
        }
    }

    ParserRegistry &ParserRegistry::instance() {
        static ParserRegistry registry;
        return registry;
    }

    void ParserRegistry::populateParser(Parser &parser) {
        for (const auto& it: _libraries) {
            it(parser);
        }
    }

    void ParserRegistry::registerLibrary(const std::function<void(Parser &)>& library) {
        _libraries.push_back(library);
    }
}
