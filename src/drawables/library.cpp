#include "library.h"

namespace drawables {

    Library::Library(std::string_view name) : _name(name) {
    }

    std::string_view Library::getName() const {
        return _name;
    }

    const std::map<std::string, std::shared_ptr<BaseDrawable>> &Library::getDrawables() const {
        return _drawables;
    }


}
