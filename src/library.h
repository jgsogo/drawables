#pragma once

#include <string_view>

#include "drawable.h"
#include "texture_loader.h"

namespace drawables {

    class Library {
    public:
        explicit Library(std::string_view name);

        static Library parse(std::string_view name, const std::filesystem::path &filename, TextureLoader &loader);

        [[nodiscard]] std::string_view getName() const { return _name; }

        [[nodiscard]] const std::map<std::string, std::shared_ptr<BaseDrawable>> &getDrawable() const { return _drawables; }

    protected:
        const std::string _name;
        std::map<std::string, std::shared_ptr<BaseDrawable>> _drawables;
    };

}
