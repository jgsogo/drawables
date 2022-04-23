#pragma once

#include <string_view>

#include "drawable.h"

namespace drawables {
    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    class Parser;

    template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
    class Library {
    public:
        using BaseDrawable = BaseDrawable<Origin, TDrawList, PixelsSymbol>;

        explicit Library(std::string_view name) : _name(name) {
        }

        [[nodiscard]] std::string_view getName() const {
            return _name;
        }

        [[nodiscard]] const std::map<std::string, std::shared_ptr<BaseDrawable>> &getDrawables() const {
            return _drawables;
        }

    protected:
        template<const char *OriginT, typename TDrawListT, const char *PixelsSymbolT>
        friend class Parser;

        const std::string _name;
        std::map<std::string, std::shared_ptr<BaseDrawable>> _drawables;
    };

}
