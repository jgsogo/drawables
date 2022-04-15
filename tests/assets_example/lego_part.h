#pragma once

#include "drawables/drawable.h"
#include "drawables/connection.h"

#include "connection.h"

namespace example {

    struct LegoPart : public drawables::Drawable<ConnectionType> {
        constexpr static char type[] = "part";
        using Connection = drawables::Connection<ConnectionType>;

        // Cross-reference
        struct Bricklink {
            std::string id; // https://www.bricklink.com/v3/api.page?page=get-item
            int colorID; // https://www.bricklink.com/v3/api.page?page=resource-representations-color
        };
        std::optional<Bricklink> bricklink;
        std::optional<std::string> ldrawFile;
    };

}