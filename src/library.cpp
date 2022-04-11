#include "library.h"

#include <vector>
#include <fstream>
#include <sstream>
#include "rapidxml.hpp"
#include "spdlog/spdlog.h"

namespace drawables {
    namespace {
        std::pair<std::string, std::shared_ptr<BaseDrawable>> parseDrawableNode(rapidxml::xml_node<> *node,
                                                                                const std::filesystem::path &filename,
                                                                                TextureLoader &loader) {
            auto type = node->first_attribute("type");
            if (type == nullptr) {
                //return madbricks::parts::parser::parsePart<madbricks::parts::Part>(node, filename, loader);
            } else if (strcmp(type->value(), "track") == 0) {
                //return madbricks::parts::parser::parsePart<madbricks::parts::train::Track>(node, filename, loader);
            } else {
                // TODO: Handle error
                std::stringstream ss;
                ss << "Parsing not implemented for type '" << type->value() << "'";
                throw std::runtime_error(ss.str());
            }
        }
    }

    Library::Library(std::string_view name) : _name(name) {

    }

    Library Library::parse(std::string_view name, const std::filesystem::path &filename, TextureLoader &loader) {
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
                auto[id, part] = parseDrawableNode(partNode, filename, loader);
                part->id = Id{std::string{name}, id};
                ret._drawables.insert(std::make_pair(id, part));  // TODO: What if I apply XSLT transformation first instead of passing the arg?
            }
            return ret;
        } else {
            // TODO: Handle error
            spdlog::error("Cannot find root node 'parts_library' in filename '{}'", filename.string());
        }
    }

}
