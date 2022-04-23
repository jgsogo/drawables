#pragma once

#include "drawables/drawable.h"
#include "drawables/connection.h"

#include "connection.h"

namespace example {

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    struct LegoPart : public drawables::Drawable<Origin, TDrawList, PixelsSymbol, ConnectionType> {
        constexpr static char type[] = "part";
        using Connection = drawables::Connection<Origin, ConnectionType>;
        using RenderContext = typename drawables::Drawable<Origin, TDrawList, PixelsSymbol, ConnectionType>::RenderContext;

        // Cross-reference
        struct Bricklink {
            std::string id; // https://www.bricklink.com/v3/api.page?page=get-item
            int colorID; // https://www.bricklink.com/v3/api.page?page=resource-representations-color
        };
        std::optional<Bricklink> bricklink;
        std::optional<std::string> ldrawFile;

        void drawConnection(RenderContext &render, const Connection &con) const override {
            render.drawCircle(Magnum::Math::Vector2{math::types::NamedUnitT<float, Origin>{0.f}, math::types::NamedUnitT<float, Origin>{0.f}},
                              math::types::NamedUnitT<float, Origin>{8.f},
                              IM_COL32(255, 255, 0, 255),
                              math::types::NamedUnitT<float, PixelsSymbol>{1.f});
        }
    };

}