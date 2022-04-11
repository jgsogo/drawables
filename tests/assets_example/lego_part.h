#pragma once

#include "drawable.h"
#include "connection.h"

namespace example {

    class LegoPart : public drawables::Drawable<ConnectionType> {
    public:
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