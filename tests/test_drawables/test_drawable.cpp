#include "catch2/catch.hpp"
#include "units/milimeters.hpp"
#include "units/pixels.hpp"
#include "units/degrees.hpp"
#include "render/context.h"

#include "parser/parser.h"

#include "mocks/mock_draw_list.hpp"


using namespace math::units;
using namespace render::units;

TEST_CASE("test_drawables/test_drawable | doRender", "[drawables/drawable]") {
    mocks::DrawList drwList;

}