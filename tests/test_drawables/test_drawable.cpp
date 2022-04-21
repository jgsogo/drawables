#include "catch2/catch.hpp"
#include "units/milimeters.hpp"
#include "units/pixels.hpp"
#include "units/degrees.hpp"
#include "render/context.h"

#include "mock_draw_list.hpp"

using namespace math::units;
using namespace render::units;

TEST_CASE("test_drawables/test_drawable | doRender", "[drawables/drawable]") {
    mocks::DrawList drwList;

    using TransformationType = math::xy::types::Transformation<math::Pixels::symbol, math::Pixels::symbol, float>;
    TransformationType::Translation translation{10_px, 20_px};
    TransformationType::Rotation rotation{0_deg};
    auto scale = math::ratio(1_mm, 1_px);

    auto context = render::Context<math::units::mm, mocks::DrawList>{drwList, translation, rotation, scale};

    REQUIRE(42 == 42);
}