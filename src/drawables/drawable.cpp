#include "drawable.h"

namespace drawables {

    void BaseDrawable::doRender(render::ImGuiContext <math::units::mm> &render, int lodLevel) const {
        auto itDataToRender = renderData.lower_bound(lodLevel);
        for (auto &it: itDataToRender->second) {
            it->doRender(render);
        }
    }

    void BaseDrawable::doRenderBBox(render::ImGuiContext <math::units::mm> &render, ImU32 color) const {
        using namespace render::imgui::units;
        render.drawRectangle(bbox, color, 1_impx);
    }

}