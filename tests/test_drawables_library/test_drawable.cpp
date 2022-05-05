#include "catch2/catch.hpp"
#include "units/milimeters.hpp"
#include "units/pixels.hpp"
#include "units/degrees.hpp"
#include "render/context.h"

#include "parser/parser.h"
#include "assets_example/library.h"

#include "mocks/mock_draw_list.hpp"
#include "mock_application.hpp"

using namespace math::units;
using namespace render::units;

TEST_CASE("test_drawables_library/test_drawable | doRender", "[parser/drawable]") {
    mocks::DrawList drwList;

    using TransformationType = math::xy::types::Transformation<math::Pixels::symbol, math::Pixels::symbol, float>;
    TransformationType::Translation translation{10_px, 20_px};
    TransformationType::Rotation rotation{0_deg};
    auto scale = math::ratio(1_mm, 1_px);

    auto context = render::Context<math::units::mm, mocks::DrawList>{drwList, translation, rotation, scale};

    int argc = 1;
    char *argv[] = {"unit_tests", nullptr};
    auto args = Magnum::Platform::Application::Arguments{argc, argv};
    auto app = TheApplication{args};

    drawables::Parser<math::units::mm, mocks::DrawList, math::Pixels::symbol> parser(app.loader);
    auto& parserRegistry = drawables::ParserRegistry<math::units::mm, mocks::DrawList, math::Pixels::symbol>::instance();
    parserRegistry.registerLibrary(example::registerParser<math::units::mm, mocks::DrawList, math::Pixels::symbol>);
    parserRegistry.populateParser(parser);
    auto library = parser.parse(ASSETS_EXAMPLE_FILENAME);
    REQUIRE(library.getDrawables().size() == 4);

    {
        mocks::DrawList drwList;
        auto ctxt = render::Context<math::units::mm, mocks::DrawList, math::units::px>{drwList};
        auto track = library.getDrawables().at("53401");
        track->doRender(ctxt);


    }
    {
        mocks::DrawList drwList;
        auto ctxt = render::Context<math::units::mm, mocks::DrawList, math::units::px>{drwList};
        auto track = library.getDrawables().at("53401");
        track->doRender(ctxt, 0);


    }
    {
        mocks::DrawList drwList;
        auto ctxt = render::Context<math::units::mm, mocks::DrawList, math::units::px>{drwList};
        auto track = library.getDrawables().at("53401");
        track->doRender(ctxt, 2);


    }
    //track->doRender(context);

    REQUIRE(42 == 42);
}