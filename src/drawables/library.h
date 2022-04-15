#pragma once

#include <string_view>

#include "drawable.h"

namespace drawables {
    class Parser;

    class Library {
    public:
        explicit Library(std::string_view name);

        [[nodiscard]] std::string_view getName() const;

        [[nodiscard]] const std::map<std::string, std::shared_ptr<BaseDrawable>> &getDrawables() const;

    protected:
        friend class Parser;

        const std::string _name;
        std::map<std::string, std::shared_ptr<BaseDrawable>> _drawables;
    };

}
