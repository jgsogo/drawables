#include "connection.h"

#include <unordered_map>

namespace drawables::parser {
    namespace {
        const std::unordered_map<std::string_view, example::ConnectionType> tableConnectionType = {
                {"TRACK",        example::ConnectionType::TRACK},
                {"TRACK_SWITCH", example::ConnectionType::TRACK_SWITCH},
        };
    }

    template<>
    example::ConnectionType parseEnum<example::ConnectionType>(std::string_view str) {
        return parseEnum(str, tableConnectionType);
    }

    template<>
    std::string_view strEnum<example::ConnectionType>(const example::ConnectionType &value) {
        return strEnum(value, tableConnectionType);
    }
}
