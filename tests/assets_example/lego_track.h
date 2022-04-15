#pragma once

#include "parser/enums.hpp"
#include "lego_part.h"

namespace example {

    enum class Voltage {
        V9, V12, V45, NONE
    };

    enum class Gauge {
        NORMAL, NARROW, MONORRAIL
    };

    struct LegoTrack : public LegoPart {
        constexpr static char type[] = "track";
        Voltage volt = Voltage::NONE;
        Gauge gauge = Gauge::NORMAL;
    };

}

namespace drawables::parser {
    // train voltage
    template<>
    example::Voltage parseEnum<example::Voltage>(std::string_view str);

    template<>
    std::string_view strEnum<example::Voltage>(const example::Voltage &value);

    // train gauge
    template<>
    example::Gauge parseEnum<example::Gauge>(std::string_view str);

    template<>
    std::string_view strEnum<example::Gauge>(const example::Gauge &value);
}
