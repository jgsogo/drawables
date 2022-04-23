#pragma once

#include <string_view>
#include <unordered_map>
#include <algorithm>

namespace drawables::parser {

    template<typename T>
    T parseEnum(std::string_view str, const std::unordered_map<std::string_view, T> &table) {
        auto it = table.find(str);
        if (it != table.end()) {
            return it->second;
        } else {
            // TODO: Handle error
            throw std::runtime_error("Enum str-value not found");
        }
    }

    template<typename T>
    std::string_view strEnum(const T &value, const std::unordered_map<std::string_view, T> &table) {
        auto found = std::find_if(table.begin(), table.end(), [&value](auto it) -> bool { return it.second == value; });
        if (found != table.end()) {
            return found->first;
        }
        // TODO: Handle error
        throw std::runtime_error("Enum value not found");
    }

    template<typename T>
    T parseEnum(std::string_view str);

    template<typename T>
    std::string_view strEnum(const T &);
}
