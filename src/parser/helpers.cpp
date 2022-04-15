#include "helpers.h"

namespace drawables::parser {

    std::optional<std::string> optionalStr(rapidxml::xml_node<> *node) {
        if (node == nullptr) {
            return std::nullopt;
        }
        return node->value();
    }

    template<>
    int stoXXX<int>(const std::string &str) {
        return std::stoi(str);
    }

    ImU32 parseColor(char *color) {
        int r, g, b;
        sscanf(color, "#%02x%02x%02x", &r, &g, &b);
        return IM_COL32(r, g, b, 255);
    }
}
