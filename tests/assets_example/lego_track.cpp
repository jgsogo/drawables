#include "lego_track.h"

namespace drawables::parser {

    namespace {
        const std::unordered_map<std::string_view, example::Voltage> tableVoltage = {
                {"V9",   example::Voltage::V9},
                {"V12",  example::Voltage::V12},
                {"V45",  example::Voltage::V45},
                {"NONE", example::Voltage::NONE},
        };
        const std::unordered_map<std::string_view, example::Gauge> tableGauge = {
                {"NORMAL",    example::Gauge::NORMAL},
                {"NARROW",    example::Gauge::NARROW},
                {"MONORRAIL", example::Gauge::MONORRAIL},
        };

    }

    // train voltage
    template<>
    inline example::Voltage parseEnum<example::Voltage>(std::string_view str) {
        return parseEnum(str, tableVoltage);
    }

    template<>
    inline std::string_view strEnum<example::Voltage>(const example::Voltage &value) {
        return strEnum(value, tableVoltage);
    }

    // train gauge
    template<>
    inline example::Gauge parseEnum<example::Gauge>(std::string_view str) {
        return parseEnum(str, tableGauge);
    }

    template<>
    inline std::string_view strEnum<example::Gauge>(const example::Gauge &value) {
        return strEnum(value, tableGauge);
    }
}
