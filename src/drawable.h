#pragma once

#include <optional>
#include <map>
#include <memory>
#include <vector>

#include <Magnum/Math/Range.h>

#include "primitives/primitive.hpp"
#include "render/imgui/context.h"
#include "render/imgui/pixels.hpp"

#include "id.h"
#include "connection.h"

namespace drawables {
    enum class LODLevel {
        LOD0,   // just bounding boxes
        LOD1,   // only lines
        LOD2    // images and textures
    };

    class Drawable {

    public:
        Drawable() = default;

        virtual ~Drawable() = default;

        void doRender(render::ImGuiContext <math::units::mm> &render, LODLevel level = LODLevel::LOD2) const;

        void doRenderConnections(render::ImGuiContext <math::units::mm> &render) const;

        void doRenderBBox(render::ImGuiContext <math::units::mm> &render, ImU32 color = IM_COL32(255, 0, 0, 255)) const;
    public:
        Id id;
        std::string name;
        std::optional <std::string> description;
        Magnum::Math::Range2D <math::Milimeters> bbox;
        std::vector <Connection> connections;

        // This is a CAD-like program, everything is to be rendered

        std::map <LODLevel, std::vector<
                std::shared_ptr < primitives::Primitive < math::units::mm, ImDrawList, render::imgui::units::impx>>>, std::greater<>> renderData;

    };
}
