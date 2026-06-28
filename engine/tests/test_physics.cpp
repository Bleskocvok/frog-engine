
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>

/**
 * Here go the tests.
 */

#include "frog/geometry/physics/physics.hpp"
#include "frog/geometry/physics/collisions.hpp"

using namespace frog::geo;


TEST_CASE("container")
{
    auto collisions = Collisions{};

    collisions.insert_collision({ 1, 2 });

    REQUIRE(collisions.contains(collisions.first(), 1));
    REQUIRE(collisions.contains(collisions.first(), 2));

    REQUIRE(collisions .contains(collisions.first(), 2));

    collisions.update(Container<Joint>(10));

    REQUIRE(collisions.contains(collisions.all(), 1));
    REQUIRE(collisions.contains(collisions.all(), 2));

    REQUIRE(collisions.contains(collisions.all_without_joints_, 1));
    REQUIRE(collisions.contains(collisions.all_without_joints_, 2));
}
