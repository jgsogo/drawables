#pragma once

#include "parser/enums.hpp"
#include "drawable.h"

namespace example {

    enum class ConnectionType {
        TRACK, TRACK_SWITCH
    };

}

namespace drawables {

    template<>
    void Drawable<example::ConnectionType>::drawConnection(render::ImGuiContext <math::units::mm> &render,
                                                           const Connection<example::ConnectionType> &con) const;

}

namespace drawables::parser {
    // train voltage
    template<>
    example::ConnectionType parseEnum<example::ConnectionType>(std::string_view str);

    template<>
    std::string_view strEnum<example::ConnectionType>(const example::ConnectionType &value);
}
