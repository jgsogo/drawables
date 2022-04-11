#include "connection.h"

#include <unordered_map>

namespace drawables {

    template<>
    void Drawable<example::ConnectionType>::drawConnection(render::ImGuiContext <math::units::mm> &render,
                                                           const Connection<example::ConnectionType> &con) const {
        using namespace math::units;
        using namespace render::imgui::units;
        render.drawCircle({0_mm, 0_mm}, 8_mm, IM_COL32(255, 255, 0, 255), 1_impx);
    }

}

namespace drawables::parser {
    namespace {
        const std::unordered_map<std::string_view, example::ConnectionType> tableConnectionType = {
                {"TRACK",        example::ConnectionType::TRACK},
                {"TRACK_SWITCH", example::ConnectionType::TRACK_SWITCH},
        };
    }

    template<>
    example::ConnectionType parseEnum<example::ConnectionType>(std::string_view str){
        return parseEnum(str, tableConnectionType);
    }

    template<>
    std::string_view strEnum<example::ConnectionType>(const example::ConnectionType &value){
        return strEnum(value, tableConnectionType);
    }
}
