#include "paddle.hpp"

#include "general.hpp"

#include <cmath>
#include <algorithm>    // std::min_element


using namespace frog::geo;


std::optional<collision_info> frog::geo::collision_2D(
                                            geo::paddle paddle,
                                            cylindrical3 coords,
                                            float radius)
{
    // first discard obvious cases when collision is impossible

    if (coords.y < paddle.pos.y || coords.y > paddle.pos.y + paddle.height)
        return std::nullopt;

    if (coords.r + radius < paddle.pos.r)
        return std::nullopt;

    if (coords.r - radius > paddle.pos.r + paddle.thickness)
        return std::nullopt;

    float diff = angle_diff(coords.theta,
                            paddle.pos.theta + paddle.angular_size / 2);

    float angular_radius = std::atan2(radius, coords.r);

    if (std::abs(diff) > paddle.angular_size / 2 + angular_radius)
        return std::nullopt;

    // check collision with the corners
    if (std::abs(diff) > paddle.angular_size / 2
        && std::abs(paddle.pos.r + paddle.thickness / 2 - coords.r)
                > paddle.thickness / 2)
    {
        // theoretically it's possible that the ball is colliding with more than
        // one corner, so we store: 1. distances squared; 2. coordinates
        // for each corner in this array
        auto dists2 = std::array<std::pair<float, vec2>, 4>{ };

        vec3 coords_cart = to_cartesian(coords);

        for (int r : { 0, 1 })
        {
            for (int alpha : { 0, 1 })
            {
                auto polar = polar2
                {
                    paddle.pos.r + r * paddle.thickness,
                    paddle.pos.theta + alpha * paddle.angular_size,
                };

                vec2 corner = to_cartesian(polar);

                float dist2 = (corner - to_vec2(coords_cart)).length_squared();

                dists2[r * 2 + alpha] = { dist2, corner };
            }
        }
        // find the closest corner to the ball
        auto closest = std::min_element(dists2.begin(), dists2.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });

        // if the closest one doesn't collide we can safely assume none
        // of them collide
        if (closest->first > radius * radius)
            return std::nullopt;

        vec3 normal = to_vec3(to_vec2(coords_cart) - closest->second)
            .normalized();

        vec3 corner3 = to_vec3(closest->second);
        corner3.y() = coords.y;

        coords_cart = corner3 + normal * (radius + Epsilon);
        coords_cart.y() = coords.y;

        return collision_info{ normal, coords_cart };
    }

    // check collision with the beginning and ending sides
    if (std::abs(diff) > paddle.angular_size / 2)
    {
        vec3 up = { 0, 1, 0 };

        vec3 vec = to_cartesian(coords);

        vec3 normal = vec.cross(up) * (diff > 0 ? -1 : 1);

        coords.theta = diff < 0
            ? paddle.pos.theta + paddle.angular_size + angular_radius + Epsilon
            : paddle.pos.theta - angular_radius - Epsilon;

        return collision_info{ normal.normalized(),
                               to_cartesian(coords) };
    }

    // check collision with inside and outside curve

    bool inside = coords.r < paddle.pos.r + paddle.thickness / 2;

    vec3 normal = to_cartesian(coords) * (inside ? -1 : 1);
    normal.y() = 0;

    coords.r = inside ? paddle.pos.r - radius - Epsilon
                      : paddle.pos.r + paddle.thickness + radius + Epsilon;

    return collision_info{ normal.normalized(),
                           to_cartesian(coords) };
}
