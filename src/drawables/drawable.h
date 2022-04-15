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

    struct BaseDrawable {
        BaseDrawable() = default;

        virtual ~BaseDrawable() = default;

        void doRender(render::ImGuiContext <math::units::mm> &render, int lodLevel = -1) const;

        virtual void doRenderConnections(render::ImGuiContext <math::units::mm> &render) const = 0;

        void doRenderBBox(render::ImGuiContext <math::units::mm> &render, ImU32 color = IM_COL32(255, 0, 0, 255)) const;

        Id id;
        std::string name;
        std::optional<std::string> description;
        Magnum::Math::Range2D <math::Milimeters> bbox;

        using Primitive = primitives::Primitive<math::units::mm, ImDrawList, render::imgui::units::impx>;
        std::map<int, std::vector<std::shared_ptr<Primitive>>, std::greater<>> renderData;
    };

    template<typename ConnectionTypes>
    struct Drawable : public BaseDrawable {
        Drawable() = default;

        ~Drawable() override = default;

        void doRenderConnections(render::ImGuiContext <math::units::mm> &render) const override {
            using namespace math::units;
            using namespace render::imgui::units;
            for (auto &con: connections) {
                auto rctxt = render << con.position;
                drawConnection(rctxt, con);
            }
        }

        static void drawConnection(render::ImGuiContext <math::units::mm> &render, const Connection<ConnectionTypes> &con) {
            using namespace math::units;
            using namespace render::imgui::units;
            render.drawCircle({0_mm, 0_mm}, 8_mm, IM_COL32_BLACK, 1_impx);
        }

    public:
        std::vector<Connection<ConnectionTypes>> connections;
    };
}
