#pragma once

#include "parser/enums.hpp"
#include "drawables/drawable.h"

namespace example {

    enum class ConnectionType {
        TRACK, TRACK_SWITCH
    };

}

namespace drawables::parser {
    template<>
    example::ConnectionType parseEnum<example::ConnectionType>(std::string_view str);

    template<>
    std::string_view strEnum<example::ConnectionType>(const example::ConnectionType &value);
}
