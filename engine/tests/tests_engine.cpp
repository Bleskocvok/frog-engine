
#include "catch.hpp"

/**
 * Here go the tests.
 */

#include "geometry/basic.hpp"
#include "geometry/vector.hpp"
#include "geometry/matrix.hpp"
#include "geometry/general.hpp"
#include "geometry/objects.hpp"
#include "geometry/ray.hpp"
#include "geometry/transform.hpp"

#include "utils/debug.hpp"

#include <iostream>
#include <functional>   // std::plus
#include <algorithm>    // std::equal


using namespace frog::geo;


TEST_CASE("constants")
{
    REQUIRE(Pi == Approx(3.14159265358));
    REQUIRE(ToDeg * Pi == Approx(180));
    REQUIRE(ToDeg * 2 * Pi == Approx(360));
    REQUIRE(ToRad * 180 == Approx(Pi));
    REQUIRE(ToRad * 360 == Approx(2 * Pi));
}


TEST_CASE("vec")
{
    vec2 ahoj = { 0, 2};
    REQUIRE(ahoj.x() == 0);
    REQUIRE(ahoj.y() == 2);

    vec2 a = { 1, 2 };
    vec2 b = { 3, 5 };
    vec2 r = a + b;
    CHECK(r.x() == 4);
    CHECK(r.y() == 7);

    CHECK(vec3(1, 2, 3) == vec3(1, 2, 3));
    CHECK(vec3(1, 3, 2) != vec3(1, 2, 3));
    CHECK(vec3(1, 3, 2) * 2 == vec3(2, 6, 4));
    vec3 two(1, 3, 2);
    two *= 2;
    CHECK(two == vec3(2, 6, 4));
    CHECK(frog::geo::is_close(two.length(), 7.483314774));
    CHECK(a.dot(b) == 13);

    CHECK(two.cross(two) == vec3(0, 0, 0));
    CHECK(vec3(1, 2, 3).cross(vec3(4, 5, 6)) == vec3(-3, 6, -3));
    CHECK(vec3(1, 0, 0).cross(vec3(0, 1, 0)) == vec3(0, 0, 1));
    CHECK(frog::geo::is_close(vec3(2, 4, 2).normalized().length(), 1));
    CHECK(vec4(3.5) == vec4(3.5, 3.5, 3.5, 3.5));
    CHECK(vec4(3) == vec4(3, 3, 3, 3));
    CHECK(-vec4(1, 2, 3, 4) == vec4(-1, -2, -3, -4));

    auto n = vec3(2, 4, 2).normalized();
    CHECK(n == vec3(2, 4, 2).normalize());
}


TEST_CASE("matrix")
{
    mat2 a = { 1, 2, 3, 4 };
    mat2 b(1, 2, 3, 4);
    a = b;
    CHECK(a.at(0, 0) == 1);
    CHECK(a.at(1, 0) == 2);
    CHECK(a.at(0, 1) == 3);
    CHECK(a.at(1, 1) == 4);
    CHECK(a + b == a * 2);
    mat2 c(1);
    CHECK(mat2(1, 2, 3, 4).transposed() == mat2(1, 3, 2, 4));
    CHECK(mat32(1, 2, 3, 4, 5, 6).transposed() == mat23(1, 4, 2, 5, 3, 6));
    {
        matrix<int, 3, 7> a
        {
            1, 2, 1,
            0, 1, 0,
            2, 1, 1,
            1, 9, 3,
            1, -6, 1,
            2, 45, 1,
            1, 3, 1
        };
        matrix<int, 2, 3> b
        {
            2, 5,
            6, 7,
            1, 8
        };
        matrix<int, 2, 7> r
        {
            15, 27,
            6, 7,
            11, 25,
            59, 92,
            -33, -29,
            275, 333,
            21, 34
        };
        CHECK(a * b == r);
    }
}


TEST_CASE("matrix vector multiplication")
{
    mat32 m = { 1, -1, 2,
                0, -3, 1 };
    vec3 v = { 2, 1, 0 };
    CHECK(m * v == vec2{ 1, -3 });
    CHECK(v * m.transposed() == vec2{ 1, -3 });
}


TEST_CASE("matrix det")
{
    CHECK(mat2{ 1, 2, 3, 4 }.submatrix(1) == matrix<float, 1, 1>{ 3 });
    CHECK(mat2{ 1, 2, 3, 4 }.submatrix(0) == matrix<float, 1, 1>{ 4 });
    CHECK(mat3{ 1, 2, 3,
                4, 5, 6,
                7, 8, 9 }.submatrix(1) == mat2{ 4, 6, 7, 9 });

    auto m2 = mat2{ 1, 2, 3, 4 };
    CHECK(m2.det() == -2);
    CHECK(mat3{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }.det() == 0);
    CHECK(mat4{
        1, 3, 5, 9,
        1, 3, 1, 7,
        4, 3, 9, 7,
        5, 2, 0, 9
    }.det() == -376);
}


TEST_CASE("matrix inversion")
{
    auto check = [](const auto& m)
    {
        decltype(m) id(1);
        if (m.det() != 0)
            REQUIRE(id.is_close(m * m.inverted()));
    };

    check(mat2{ 1, 2, 3, 4 });
    check(mat3{ 1, 3, 5,
                9, 1, 3,
                1, 7, 4 });

    for (int i = 0; i < 100; i++)
    {
        auto f = static_cast<float>(i);
        check(mat2{ f });
        check(mat3{ f });
        // check(mat4{ f });
    }
}


TEST_CASE("ode solver")
{
    auto check = [&](const auto& sols, const auto& expected)
    {
        CHECK(std::equal(
            sols.begin(),
            sols.end(),
            expected.begin(),
            expected.end(),
            [](auto a, auto b)
                {
                    a.first = std::round(a.first * 1000.0) / 1000.0;
                    a.second = std::round(b.second * 1000.0) / 1000.0;
                    return is_close(a.first, b.first)
                        && is_close(a.second, b.second);
                }));
    };

    {
        auto sols = ode_solutions(std::plus<float>{}, 0, 1, 0.1, 1);
        auto expected = std::vector<std::pair<float, float>>
        {
            { 0, 1 },
            { 0.1, 1.1 },
            { 0.2, 1.22 },
            { 0.3, 1.362 },
            { 0.4, 1.528 },
            { 0.5, 1.721 },
            { 0.6, 1.943 },
            { 0.7, 2.197 },
            { 0.8, 2.487 },
            { 0.9, 2.816 },
            { 1.0, 3.187 },
        };
        check(sols, expected);
    }

    {
        auto sols = ode_solutions([](auto, auto b){ return b; }, 0, 1, 1, 3);
        auto expected = std::vector<std::pair<float, float>>
        {
            { 0, 1 },
            { 1, 2 },
            { 2, 4 },
            { 3, 8 },
        };
        check(sols, expected);
    }
}


TEST_CASE("distance")
{
    // points
    CHECK(distance(vec3(0, 1, 0), vec3(0, 0, 0)) == Approx(1));
    CHECK(distance(vec3(0, 1, 0), vec3(1, 0, 0)) == Approx(std::sqrt(2)));
    CHECK(distance(vec3(0, 1, 0), vec3(0, 0, 1)) == Approx(std::sqrt(2)));
    CHECK(distance(vec3(0, 0, 0), vec3(1, 1, 1)) == Approx(std::sqrt(3)));

    // rays
    CHECK(distance(ray3(0, 0, 0, 1, 0, 0), ray3(0, 1, 0, 1, 0, 1)) == Approx(1));
    CHECK(distance(ray3(0, 0, 0, 1, 0, 0), ray3(0, 0, 0, 1, 0, 1)) == Approx(0));
    CHECK(distance(ray3(0, 0, 0, 1, 0, 0), ray3(0, 0, 0, 1, 0, 0)) == Approx(0));
    CHECK(distance(ray3(0, 0, 0, 1, 0, 0), ray3(0, 3, 0, 1, 0, 1)) == Approx(3));
    CHECK(distance(ray3(0, 0, 0, 1, 0, 0), ray3(0, 0, 0, 0, 1, 0)) == Approx(0));
    CHECK(distance(ray3(1, 2, 3, 1, 0, 0), ray3(2, 4, 6, 1, 2, 3)) == Approx(0));
    CHECK(distance(ray3(0, 0, 0, 1, 0, 0), ray3(0, 3, 0, 23, 0, 23)) == Approx(3));
    CHECK(distance(ray3(0, 0, -3, 12, 2, 0), ray3(0, 0, 3, 23, 67, 0)) == Approx(6));

    // ray vs point
    CHECK(distance(ray3(0, 0, 0, -1, 1, 0), vec3(1, 1, 0)) == Approx(std::sqrt(2)));

    // ray intersection
    CHECK(intersection(ray3(0, 0, 0, 1, 2, 3), ray3(0, 0, 0, 4, 5, 6))->is_close(vec3{ 0, 0, 0 }));
    auto a = ray3(0, 0, 0, 3, 0, 0),
         b = ray3(1, 0, 0, 4, 0, 0);
    auto inter = intersection(a, b);
    REQUIRE(inter.operator bool());
    CHECK(distance(*inter, a) == Approx(0));
    CHECK(distance(*inter, b) == Approx(0));

    // ray close point
    CHECK(closest_point(ray3(2, 0, 0, 0, 1, 0), vec3(2, 1, 0)) == vec3(2, 1, 0));
}


TEST_CASE("collisions")
{
    {
        auto sph1 = sphere{ 0, 0, 0, 1 };
        auto box1 = aabb{ 0, 0, 0, 1, 1, 1 };
        CHECK(sph1.collides(box1));
        CHECK(box1.collides(sph1));

        CHECK(sph1.collides(sph1));
        CHECK(box1.collides(box1));

        CHECK(sph1.collides(sphere{ 0.75, 0, 0, 0.1 }));
        CHECK(sph1.collides(sphere{ 1.01, 1.01, 1.01, 1 }));
        CHECK(sph1.collides(sphere{ 3, 4, 3, 2 }) == false);

        CHECK(box1.collides(aabb{ 0.95, 0.95, 0.95, 1, 1, 1 }));
        CHECK(box1.collides(aabb{ 1.1, 1.1, 1.1, 1, 1, 1 }) == false);
    }

    {
        auto check = [&](auto a, auto b, bool collide)
        {
            CHECK(a.collides(a));
            CHECK(b.collides(b));
            CHECK(a.collides(b) == collide);
            CHECK(b.collides(a) == collide);
        };

        check(aabb{ 1, 0, 0, 2, 3, 4 }, aabb{ 2.99, 0, 0, 2, 3, 4 }, true);
        check(aabb{ 1, 0, 0, 2, 3, 4 }, aabb{ 3.01, 0, 0, 2, 3, 4 }, false);

        check(aabb{ 0, -1.49, 0, 2, 3, 4 }, aabb{ 0, 1.49, 0, 2, 3, 4 }, true);
        check(aabb{ 0, -1.51, 0, 2, 3, 4 }, aabb{ 0, 1.51, 0, 2, 3, 4 }, false);

        check(aabb{ 0, 0, -1.99, 2, 3, 4 }, aabb{ 0, 0, 1.99, 2, 3, 4 }, true);
        check(aabb{ 0, 0, -2.01, 2, 3, 4 }, aabb{ 0, 0, 2.01, 2, 3, 4 }, false);

        check(aabb{ 1, -1.49, -1.99, 2, 3, 4 }, aabb{ 1.99, 1.49, 1.99, 2, 3, 4 }, true);
        check(aabb{ 1, -1.51, -2.01, 2, 3, 4 }, aabb{ 2.01, 1.51, 2.01, 2, 3, 4 }, false);
    }

    // advanced
    {
        auto box1 = aabb{ 0, 0, 0, 1, 1, 1 };
        CHECK(box1.collides(sphere{ 0.55, 0.25, 0.25, 0.04 }) == false);
        CHECK(box1.collides(sphere{ 0.55, 0.55, 0.25, 0.04 }) == false);
        CHECK(box1.collides(sphere{ 0.55, 0.55, 0.55, 0.04 }) == false);
        CHECK(box1.collides(sphere{ 0.5,  0.25, 0.25, 0.04 }));
        CHECK(box1.collides(sphere{ 0.5,  0.5,  0.25, 0.04 }));
        CHECK(box1.collides(sphere{ 0.5,  0.5,  0.5,  0.04 }));
        CHECK(box1.collides(sphere{ 0.53, 0.5,  0.5,  0.04 }));
        CHECK(box1.collides(sphere{ 0.5,  0.53, 0.5,  0.04 }));
        CHECK(box1.collides(sphere{ 0.5,  0.53, 0.53, 0.04 }));
    }
}


TEST_CASE("transform")
{
    auto a = translate({ 2, 4, 6 }) * vec4(1, 2, 3, 1);
    CHECK(a.is_close({ 3, 6, 9, 1 }));
}


TEST_CASE("reflection")
{
    CHECK(reflect({ 0.2, -0.4, 0   }, { 0,  1, 0 }).is_close({ 0.2, 0.4, 0   }));
    CHECK(reflect({ 0.2, -0.4, 0   }, { 0, -1, 0 }).is_close({ 0.2, 0.4, 0   }));
    CHECK(reflect({ 0,   -0.4, 0.3 }, { 0,  1, 0 }).is_close({ 0,   0.4, 0.3 }));
    CHECK(reflect({ 0,   -0.4, 0.3 }, { 0, -1, 0 }).is_close({ 0,   0.4, 0.3 }));
}
