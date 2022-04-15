#pragma once

#include "units/milimeters.hpp"
#include "transformation/transformation.hpp"

namespace drawables {

    template <typename Type>
    struct Connection {
        /*
        enum class Type {
            TRACK, TRACK_SWITCH
        };
         */
        Type type;
        math::xy::types::Transformation<math::units::mm, math::units::mm, float> position;
    };

}
