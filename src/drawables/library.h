#pragma once

#include <string_view>

#include "drawable.h"

namespace drawables {
    class Parser;

    class Library {
    public:
        explicit Library(std::string_view name);

        [[nodiscard]] std::string_view getName() const { return _name; }

        [[nodiscard]] const std::map<std::string, std::shared_ptr<BaseDrawable>> &getDrawables() const { return _drawables; }

    protected:
        friend class Parser;
        const std::string _name;
        std::map<std::string, std::shared_ptr<BaseDrawable>> _drawables;
    };

}
