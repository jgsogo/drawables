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

        REQUIRE(drwList.drawCircle.empty());
        REQUIRE(drwList.drawLine.size() == 2);
        REQUIRE(drwList.drawRectangle.empty());
        REQUIRE(drwList.drawRectangleFilled.empty());
        REQUIRE(drwList.drawPolyline.size() == 1);
        REQUIRE(drwList.drawPolylineFilled.size() == 1);
        REQUIRE(drwList.drawText.size() == 1);
        REQUIRE(drwList.drawImage.empty());
    }
    {
        mocks::DrawList drwList;
        auto ctxt = render::Context<math::units::mm, mocks::DrawList, math::units::px>{drwList};
        auto track = library.getDrawables().at("53401");
        track->doRender(ctxt, 2);

        REQUIRE(drwList.drawCircle.empty());
        REQUIRE(drwList.drawLine.empty());
        REQUIRE(drwList.drawRectangle.empty());
        REQUIRE(drwList.drawRectangleFilled.empty());
        REQUIRE(drwList.drawPolyline.empty());
        REQUIRE(drwList.drawPolylineFilled.empty());
        REQUIRE(drwList.drawText.empty());
        REQUIRE(drwList.drawImage.size() == 1);
    }
}
