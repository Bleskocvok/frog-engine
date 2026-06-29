#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

/**
 * Here go the tests.
 */

#include "frog/geometry/physics/collisions.hpp"
#include "frog/geometry/physics/primitives.hpp"

using namespace frog::geo;

#include <set>

TEST_CASE("container_set")
{
    detail::ContainsInfo::reset();

    auto collisions = Collisions<std::set>{};

    collisions.reset();

    collisions.insert_collision({ 1, 2 });

    CHECK(collisions.contains(collisions.first(), 1));
    CHECK(collisions.contains(collisions.first(), 2));

    auto joints = Container<Joint>(10);
    joints.emplace(Joint{ .a = 3, .b = 4, .dist = 1 });

    collisions.update(joints);

    CHECK(collisions.contains(collisions.all(), 1));
    CHECK(collisions.contains(collisions.all(), 2));

    CHECK(collisions.contains(collisions.all_without_joints_, 1));
    CHECK(collisions.contains(collisions.all_without_joints_, 2));

    collisions.reset();

    collisions.insert_collision({ 3, 1 });
    collisions.insert_collision({ 4, 3 });

    collisions.update(joints);

    CHECK(not collisions.contains(collisions.all_without_joints_, 2));
    CHECK(not collisions.contains(collisions.all_without_joints_, 4));
    CHECK(collisions.contains(collisions.all_without_joints_, 1));
    CHECK(collisions.contains(collisions.all_without_joints_, 3));

    CHECK(collisions.contains(collisions.all(), 3));
    CHECK(collisions.contains(collisions.all(), 4));
    CHECK(collisions.contains(collisions.all(), 1));
    CHECK(not collisions.contains(collisions.all(), 2));

    CHECK(collisions.contains(collisions.first(), 3));
    CHECK(collisions.contains(collisions.first(), 1));
    CHECK(not collisions.contains(collisions.first(), 2));
    CHECK(collisions.contains(collisions.first(), 4));

    CHECK(collisions.contains(collisions.current(), 1));
    CHECK(collisions.contains(collisions.current(), 3));
    CHECK(collisions.contains(collisions.current(), 4));
    CHECK(not collisions.contains(collisions.current(), 2));

    CHECK(detail::ContainsInfo::set_count() > 0);
    CHECK(detail::ContainsInfo::bag_count() == 0);
}

TEST_CASE("container_bag")
{
    detail::ContainsInfo::reset();

    auto collisions = Collisions<CollisionBag>{};

    collisions.reset();

    collisions.insert_collision({ 1, 2 });

    CHECK(collisions.contains(collisions.first(), 1));
    CHECK(collisions.contains(collisions.first(), 2));

    auto joints = Container<Joint>(10);
    joints.emplace(Joint{ .a = 3, .b = 4, .dist = 1 });

    collisions.update(joints);

    CHECK(collisions.contains(collisions.all(), 1));
    CHECK(collisions.contains(collisions.all(), 2));

    CHECK(collisions.contains(collisions.all_without_joints_, 1));
    CHECK(collisions.contains(collisions.all_without_joints_, 2));

    collisions.reset();

    collisions.insert_collision({ 3, 1 });
    collisions.insert_collision({ 4, 3 });

    collisions.update(joints);

    CHECK(not collisions.contains(collisions.all_without_joints_, 2));
    CHECK(not collisions.contains(collisions.all_without_joints_, 4));
    CHECK(collisions.contains(collisions.all_without_joints_, 1));
    CHECK(collisions.contains(collisions.all_without_joints_, 3));

    CHECK(collisions.contains(collisions.all(), 3));
    CHECK(collisions.contains(collisions.all(), 4));
    CHECK(collisions.contains(collisions.all(), 1));
    CHECK(not collisions.contains(collisions.all(), 2));

    CHECK(collisions.contains(collisions.first(), 3));
    CHECK(collisions.contains(collisions.first(), 1));
    CHECK(not collisions.contains(collisions.first(), 2));
    CHECK(collisions.contains(collisions.first(), 4));

    CHECK(collisions.contains(collisions.current(), 1));
    CHECK(collisions.contains(collisions.current(), 3));
    CHECK(collisions.contains(collisions.current(), 4));
    CHECK(not collisions.contains(collisions.current(), 2));

    CHECK(detail::ContainsInfo::bag_count() > 0);
    CHECK(detail::ContainsInfo::set_count() == 0);
}
