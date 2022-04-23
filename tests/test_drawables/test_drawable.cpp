#include "catch2/catch.hpp"
#include "units/milimeters.hpp"
#include "units/pixels.hpp"
#include "units/degrees.hpp"
#include <primitives/line.hpp>
#include <primitives/rectangle.hpp>
#include "render/context.h"

#include "parser/parser.h"

#include "mocks/mock_draw_list.hpp"


using namespace math::units;
using namespace render::units;

enum class ConnectionTypeTest {
    CON1, CON2
};

TEST_CASE("test_drawables/test_drawable | doRender", "[drawables/drawable]") {
    mocks::DrawList drwList;
    auto ctxt = render::Context<math::units::mm, mocks::DrawList, math::units::px>{drwList};

    SECTION("Empty drawable") {
        drawables::Drawable<math::units::mm, mocks::DrawList, math::units::px, ConnectionTypeTest> drw;
        drw.doRender(ctxt);
        drw.doRender(ctxt, 1);
        drw.doRender(ctxt, 100);
        drw.doRenderConnections(ctxt);
        REQUIRE(drwList.drawCircle.empty());
        REQUIRE(drwList.drawLine.empty());
        REQUIRE(drwList.drawRectangle.empty());
        REQUIRE(drwList.drawRectangleFilled.empty());
        REQUIRE(drwList.drawPolyline.empty());
        REQUIRE(drwList.drawPolylineFilled.empty());
        REQUIRE(drwList.drawText.empty());
        REQUIRE(drwList.drawImage.empty());

        drw.doRenderBBox(ctxt);
        REQUIRE(drwList.drawPolyline.size() == 1);
    }

    SECTION("LOD levels") {
        drawables::Drawable<math::units::mm, mocks::DrawList, math::units::px, ConnectionTypeTest> drw;
        // LOD0 - just a line
        {
            auto &lod0 = drw.renderData[0];
            auto line = std::make_shared<primitives::Line<math::units::mm, mocks::DrawList, math::units::px>>();
            line->color = IM_COL32_BLACK;
            line->thickness = 2_px;
            line->start = Magnum::Math::Vector2{0_mm, 0_mm};
            line->end = Magnum::Math::Vector2{10_mm, 0_mm};
            lod0.push_back(line);
        }

        // LOD5 - A circle
        {
            auto &lod5 = drw.renderData[5];
            auto rect = std::make_shared<primitives::Rectangle<math::units::mm, mocks::DrawList, math::units::px>>();
            rect->rect = Magnum::Math::Range2D{Magnum::Math::Vector2{0_mm, 0_mm}, Magnum::Math::Vector2{10_mm, 10_mm}};
            lod5.push_back(rect);
        }

        REQUIRE(drwList.drawLine.empty());
        drw.doRender(ctxt); // By default takes lower lod
        REQUIRE(drwList.drawLine.size() == 1);
        REQUIRE(drwList.drawRectangle.empty());

        drw.doRender(ctxt, 4);  // Not enough to jump to next LOD
        REQUIRE(drwList.drawLine.size() == 2);
        REQUIRE(drwList.drawRectangle.empty());

        drw.doRender(ctxt, 5);  // LOD5
        REQUIRE(drwList.drawLine.size() == 2);
        REQUIRE(drwList.drawPolyline.size() == 1);

        drw.doRender(ctxt, 100);  // More than available
        REQUIRE(drwList.drawLine.size() == 2);
        REQUIRE(drwList.drawPolyline.size() == 2);
    }
}