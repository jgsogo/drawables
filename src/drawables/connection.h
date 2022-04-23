#pragma once

#include "transformation/transformation.hpp"

namespace drawables {

    template<const char *symbolOrigin, typename Type>
    struct Connection {
        Type type;
        math::xy::types::Transformation<symbolOrigin, symbolOrigin, float> position{};
    };

}
