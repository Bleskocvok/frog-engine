
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

using doctest::Approx;

/**
 * Here go the tests.
 */

#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"


using namespace frog::gx;
using namespace frog::geo;


auto check_close = [](vec4 vec, vec4 expected)
{
    CHECK( vec.r() == Approx( expected.r() ).epsilon(0.01) );
    CHECK( vec.g() == Approx( expected.g() ).epsilon(0.01) );
    CHECK( vec.b() == Approx( expected.b() ).epsilon(0.01) );
    CHECK( vec.a() == Approx( expected.a() ).epsilon(0.01) );
};

TEST_CASE("conversion")
{
    auto c1 = rgba_t{ 255, 0, 0, 127 };
    auto c2 = rgba_t{ 0, 127, 127, 0 };
    auto c3 = rgba_t{ 56, 43, 189, 0 };

    float half = 127 / 255.0;
    check_close( rgb_to_vec(c1), { 1, 0, 0, half });
    check_close( rgb_to_vec(c2), { 0, half, half, 0 });
    check_close( rgb_to_vec(c3), { 0.220, 0.169, 0.741, 0 });

    CHECK_EQ( vec_to_rgb( rgb_to_vec( c1 ) ), c1 );
    CHECK_EQ( vec_to_rgb( rgb_to_vec( c2 ) ), c2 );
    CHECK_EQ( vec_to_rgb( rgb_to_vec( c3 ) ), c3 );
}

TEST_CASE("from_hsv")
{
    check_close( hsv_to_vec( { 79, 1, 1, 0.6 }),
                 rgb_to_vec( { 174, 255, 0, 255 * 0.6 } ) );

    check_close( hsv_to_vec( { 79, 0.5, 0.7, 1 }),
                 rgb_to_vec( { 150, 178, 89, 255 } ) );
}

TEST_CASE("to_hsv")
{
    check_close( vec_to_hsv( rgb_to_vec( { 174, 255, 0, 255 * 0.6 } ) ),
                { 79, 1, 1, 0.6 });
}
