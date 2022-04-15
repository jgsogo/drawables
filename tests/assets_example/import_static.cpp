
#include <spdlog/spdlog.h>

#include "parser/parser.h"
#include "library.h"

namespace example {

    static int import_static() {
        spdlog::info("Import static 'asests_example'\n");
        auto &registry = drawables::ParserRegistry::instance();
        registry.registerLibrary(registerParser);
        return 0;
    }

    namespace {
        struct Initializer_1 {
            [[maybe_unused]] static const int i;
        };
        [[maybe_unused]] const int Initializer_1::i = import_static();
    }

}
