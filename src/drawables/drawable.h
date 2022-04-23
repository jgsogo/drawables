#pragma once

#include <optional>
#include <map>
#include <memory>
#include <vector>

#include <Magnum/Math/Range.h>

#include "primitives/primitive.hpp"

#include "id.h"
#include "connection.h"

namespace drawables {

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    struct BaseDrawable {
        Id id;
        std::string name;
        std::optional<std::string> description;
        Magnum::Math::Range2D<math::types::NamedUnitT<float, Origin>> bbox;

        using Primitive = primitives::Primitive<Origin, TDrawList, PixelsSymbol>;
        using RenderContext = typename Primitive::RenderContext;
        std::map<int, std::vector<std::shared_ptr<Primitive>>, std::less<>> renderData;

        void doRender(RenderContext &render, int lodLevel = -1) const {
            // Use LOD level available that is EQUAL OR LESS to the value requested (fallback to lowest)
            if (renderData.empty()) return;

            auto selected = --(renderData.rend());
            auto it = renderData.rbegin();
            while (it != renderData.rend()) {
                if (it->first > lodLevel) {
                    ++it;
                    continue;
                }
                selected = it;
                break;
            }
            for (auto &it_drw: selected->second) {
                it_drw->doRender(render);
            }
        }

        void doRenderBBox(RenderContext &render, ImU32 color = IM_COL32(255, 0, 0, 255)) const {
            using namespace render::imgui::units;
            render.drawRectangle(bbox, color, math::types::NamedUnitT<float, PixelsSymbol>{1});
        }

        virtual void doRenderConnections(RenderContext &render) const = 0;
    };

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol, typename ConnectionTypes>
    struct Drawable : public BaseDrawable<Origin, TDrawList, PixelsSymbol> {
        std::vector<Connection<Origin, ConnectionTypes>> connections;
        using RenderContext = typename BaseDrawable<Origin, TDrawList, PixelsSymbol>::RenderContext;

        void doRenderConnections(RenderContext &render) const override {
            for (auto &con: connections) {
                auto rctxt = render << con.position;
                this->drawConnection(rctxt, con);
            }
        }

        virtual void drawConnection(RenderContext &render, const Connection<Origin, ConnectionTypes> &con) const {
            render.drawCircle(Magnum::Math::Vector2{math::types::NamedUnitT<float, Origin>{0.f}, math::types::NamedUnitT<float, Origin>{0.f}},
                              math::types::NamedUnitT<float, Origin>{8.f},
                              IM_COL32_BLACK,
                              math::types::NamedUnitT<float, PixelsSymbol>{1.f});
        }
    };

}
