#pragma once

#include <optional>
#include <string>

#include <rapidxml.hpp>
#include <Magnum/Math/Range.h>
#include <imgui.h>


namespace drawables::parser {

    std::optional<std::string> optionalStr(rapidxml::xml_node<> *node) {
        if (node == nullptr) {
            return std::nullopt;
        }
        return node->value();
    }

    template<class T>
    T stoXXX(const std::string &str) {
        return T{std::stof(str)};
    }

    template<>
    int stoXXX<int>(const std::string &str) {
        return std::stoi(str);
    }

    template<class T>
    Magnum::Math::Range2D <T> parseRange(rapidxml::xml_node<> *node) {
        T x_min = stoXXX<T>(node->first_node("x_min")->value());
        T y_min = stoXXX<T>(node->first_node("y_min")->value());
        T x_max = stoXXX<T>(node->first_node("x_max")->value());
        T y_max = stoXXX<T>(node->first_node("y_max")->value());
        Magnum::Math::Range2D <T> ret{Magnum::Math::Vector2 < T > {x_min, y_min}, Magnum::Math::Vector2 < T > {x_max, y_max}};
        return ret;
    }

    template<class T>
    Magnum::Math::Range2D <T> parseUVRange(rapidxml::xml_node<> *node, int width, int height) {
        T x_min{stoXXX<float>(node->first_node("x_min")->value()) / width};
        T y_min{1.f - stoXXX<float>(node->first_node("y_min")->value()) / height};
        T x_max{stoXXX<float>(node->first_node("x_max")->value()) / width};
        T y_max{1.f - stoXXX<float>(node->first_node("y_max")->value()) / height};
        Magnum::Math::Range2D <T> ret{Magnum::Math::Vector2 < T > {x_min, y_min}, Magnum::Math::Vector2 < T > {x_max, y_max}};
        return ret;
    }

    template<class T>
    Magnum::Math::Vector2 <T> parsePoint(rapidxml::xml_node<> *node) {
        auto x = stoXXX<T>(node->first_attribute("x")->value());
        auto y = stoXXX<T>(node->first_attribute("y")->value());
        return {x, y};
    }

    ImU32 parseColor(char *color) {
        int r, g, b;
        sscanf(color, "#%02x%02x%02x", &r, &g, &b);
        return IM_COL32(r, g, b, 255);
    }

}
