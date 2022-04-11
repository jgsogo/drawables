#include "drawable.h"

namespace drawables {

    using namespace math::units;
    using namespace render::imgui::units;

    void Drawable::doRender(render::ImGuiContext <math::units::mm> &render, LODLevel level) const {
        auto itDataToRender = renderData.lower_bound(level);
        for (auto &it: itDataToRender->second) {
            it->doRender(render);
        }
    }

    void Drawable::doRenderConnections(render::ImGuiContext <math::units::mm> &render) const {
        for (auto &con: connections) {
            (render << con.position).drawCircle({0_mm, 0_mm}, 8_mm, IM_COL32_BLACK, 1_impx);
        }
    }

    void Drawable::doRenderBBox(render::ImGuiContext <math::units::mm> &render, ImU32 color) const {
        render.drawRectangle(bbox, color, 1_impx);
    }

}
