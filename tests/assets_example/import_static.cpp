
#include <spdlog/spdlog.h>

#include "parser/parser.h"
#include "library.h"

namespace example {

    namespace {
        template<const char *Origin, typename TDrawList, const char *PixelsSymbol>
        static int import_static() {
            spdlog::info("Import static 'asests_example'\n");
            auto &registry = drawables::ParserRegistry<Origin, TDrawList, PixelsSymbol>::instance();
            registry.registerLibrary(registerParser<Origin, TDrawList, PixelsSymbol>);
            return 0;
        }
    }

    namespace {
        struct Initializer_ImGui {
            [[maybe_unused]] static const int i;
        };
        [[maybe_unused]] const int Initializer_ImGui::i = import_static<math::units::mm, ImDrawList, render::imgui::units::impx>();
    }

}
