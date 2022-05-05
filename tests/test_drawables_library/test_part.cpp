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

TEST_CASE("test_drawables_library/test_part | doRender", "[parser/drawable]") {
    int argc = 1;
    char *argv[] = {"unit_tests", nullptr};
    auto args = Magnum::Platform::Application::Arguments{argc, argv};
    auto app = TheApplication{args};

    drawables::Parser<math::units::mm, mocks::DrawList, math::Pixels::symbol> parser(app.loader);
    auto &parserRegistry = drawables::ParserRegistry<math::units::mm, mocks::DrawList, math::Pixels::symbol>::instance();
    parserRegistry.registerLibrary(example::registerParser<math::units::mm, mocks::DrawList, math::Pixels::symbol>);
    parserRegistry.populateParser(parser);
    auto library = parser.parse(ASSETS_EXAMPLE_FILENAME);
    REQUIRE(library.getDrawables().size() == 4);

    {
        auto drw = library.getDrawables().at("53401");
        REQUIRE(drw->id.library_name == "Official LEGO tracks");
        REQUIRE(drw->id.id == "53401");
        REQUIRE(drw->name == "Train, Track Plastic (RC Trains) Straight");
        REQUIRE(drw->description == "Train, Track Plastic (RC Trains) Straight");

        auto legoPart = std::static_pointer_cast<example::LegoPart<math::units::mm, mocks::DrawList, math::Pixels::symbol>>(drw);
        //REQUIRE(legoPart->bricklink->colorID == 85);  // TODO: Enable after parsing these data
        REQUIRE(legoPart->ldrawFile == "53401.dat");

        auto legoTrack = std::static_pointer_cast<example::LegoTrack<math::units::mm, mocks::DrawList, math::Pixels::symbol>>(legoPart);
        REQUIRE(legoTrack->gauge == example::Gauge::NORMAL);
        REQUIRE(legoTrack->volt == example::Voltage::V9);
    }

}
