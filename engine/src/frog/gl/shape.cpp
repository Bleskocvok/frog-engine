#ifndef NOT_FROG_BUILD_3D

#include "shape.hpp"

#include "frog/geometry/polar.hpp"
#include "frog/geometry/basic.hpp"
#include "mesh.hpp"

#include <vector>
#include <array>
#include <cmath>
#include <cassert>


using namespace frog;
using frog::geo::vec3;
using frog::gl::mesh;


mesh frog::gl::shape::paddle(int sides,
                             float radius,
                             float thickness,
                             float height,
                             float theta)
{
    using namespace geo;

    auto vertices = std::vector<float>{};
    auto normals = std::vector<float>{};
    auto tex_coords = std::vector<float>{};
    auto indices = std::vector<uint32_t>{};

    size_t last_index = 0;

    // convert angle size to radians
    theta *= geo::ToRad;

    auto add_v = [&](vec3 v, vec3 n)
    {
        vertices.push_back(v.x());
        vertices.push_back(v.y());
        vertices.push_back(v.z());

        normals.push_back(n.x());
        normals.push_back(n.y());
        normals.push_back(n.z());

        // TODO proper coordinates
        tex_coords.push_back(0.5);
        tex_coords.push_back(0.5);
    };

    using std::vector, std::array;

    static const auto triangles = array<vector<vector<int>>, 2>
    {
    // not sure why this level of brackets is necessary
    //  |
    //  V
        {
            // normal order
            { { 0, 1, 2 }, { 1, 3, 2 } },
            // reverse order
            { { 2, 1, 0 }, { 2, 3, 1 } },
        }
    };

    // t = top, b = bottom, l = left, r = right
    auto add_side = [&](vec3 tl, vec3 tr, vec3 bl, vec3 br)
    {
        vec3 normal = (tr - tl).cross(bl - tl).normalized();
        add_v(tl, normal);
        add_v(tr, normal);
        add_v(bl, normal);
        add_v(br, normal);

        for (const auto& tri : triangles[0])
        {
            for (int idx : tri)
            {
                indices.push_back(last_index + idx);
            }
        }
        last_index += 4;
    };

    auto angle = [&](int i) { return i * theta / sides; };

    auto convert = [&](float r, int i, float y)
    {
        return geo::to_cartesian(cylindrical3{ r, angle(i), y });
    };

    // add top and bottom sides
    for (int i = 0; i < sides; i++)
    {
        vec3 a = convert(radius,             i + 1, height),
             b = convert(radius + thickness, i + 1, height),
             c = convert(radius,             i,     height),
             d = convert(radius + thickness, i,     height);
        add_side(a, b, c, d);

        d.y() = c.y() = b.y() = a.y() = 0;
        // order changed because of face culling
        add_side(c, d, a, b);
    }

    // add inner and outer sides
    for (int dist : { 0, 1 })
    {
        for (int i = 0; i < sides + 1; i++)
        {
            vec3 pos = convert(radius + thickness * dist, i, 0);

            // invert either 1 or -1
            float invert = dist * 2 - 1;

            vec3 normal = vec3{ invert * pos.x(), 0, invert * pos.z() }
                .normalized();
            add_v(pos, normal);
            pos.y() = height;
            add_v(pos, normal);
        }

        for (int i = 0; i < sides; i++)
        {
            for (const auto& tri : triangles[1 - dist])
            {
                for (int idx : tri)
                {
                    indices.push_back(last_index + idx);
                }
            }
            last_index += 2;
        }
        last_index += 2;
    }

    // add front and back sides
    // front
    {
        vec3 a = convert(radius,             0, height),
             b = convert(radius + thickness, 0, height),
             c = convert(radius,             0,      0),
             d = convert(radius + thickness, 0,      0);
        add_side(a, b, c, d);
    }
    // back
    {
        vec3 a = convert(radius,             sides, height),
             b = convert(radius + thickness, sides, height),
             c = convert(radius,             sides,      0),
             d = convert(radius + thickness, sides,      0);
        // order changed because of face culling
        add_side(c, d, a, b);
    }

    return mesh(std::move(vertices)
              , std::move(normals)
              , std::move(tex_coords)
              , std::move(indices));
}


mesh frog::gl::shape::polygon_2D(int sides, float radius)
{
    using namespace geo;

    auto vertices = std::vector<float>{};
    auto normals = std::vector<float>{};
    auto tex_coords = std::vector<float>{};
    auto indices = std::vector<uint32_t>{};

    float angle = 2 * geo::Pi / sides;

    // add middle
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(0);
    normals.push_back(0);
    normals.push_back(1);
    normals.push_back(0);
    tex_coords.push_back(0.5);
    tex_coords.push_back(0.5);

    // add vertices
    for (int i = 0; i < sides; i++)
    {
        vec2 pt = geo::to_cartesian(polar2{ radius, angle * i });
        vertices.push_back(pt.x());
        vertices.push_back(0);
        vertices.push_back(pt.y());
        normals.push_back(0);
        normals.push_back(1);
        normals.push_back(0);
        tex_coords.push_back(0.5 + 0.5 * pt.x() / radius);
        tex_coords.push_back(0.5 + 0.5 * pt.y() / radius);
    }

    // add indices
    for (int i = 0; i < sides; i++)
    {
        indices.push_back((i + 2) % sides + 1);
        indices.push_back((i + 1) % sides + 1);
        indices.push_back(0);
    }

    return mesh(std::move(vertices)
              , std::move(normals)
              , std::move(tex_coords)
              , std::move(indices));
}


mesh frog::gl::shape::sphere(int rings, int sides, float radius)
{
    assert(radius != 0);

    using namespace geo;

    auto vertices = std::vector<float>{};
    auto normals = std::vector<float>{};
    auto tex_coords = std::vector<float>{};
    auto indices = std::vector<uint32_t>{};

    float y_angle = 2 * geo::Pi / static_cast<float>(sides);

    int point_idx = 0;
    auto add = [&](vec3 pt)
    {
        vertices.push_back(pt.x());
        vertices.push_back(pt.y());
        vertices.push_back(pt.z());
        normals.push_back(pt.x() / radius);
        normals.push_back(pt.y() / radius);
        normals.push_back(pt.z() / radius);
        // TODO
        tex_coords.push_back(0);
        tex_coords.push_back(0);
        indices.push_back(point_idx);
        ++point_idx;
    };

    auto add_cyl = [&](cylindrical3 cyl) { add(to_cartesian(cyl)); };

    for (int s = 0; s < sides; s++)
    {
        float ry = s * y_angle;
        float ry_next = (s + 1) * y_angle;

        for (int r = 0; r < rings; r++)
        {
            float y = radius * std::cos(r * Pi / rings);
            float y_next = radius * std::cos((r + 1) * Pi / rings);

            float r1 = std::sqrt(radius * radius - y * y);
            float r_next = std::sqrt(radius * radius - y_next * y_next);

            add_cyl({ r_next, ry_next, y_next });
            add_cyl({ r_next, ry, y_next });
            add_cyl({ r1, ry, y });

            add_cyl({ r1, ry_next, y });
            add_cyl({ r_next, ry_next, y_next });
            add_cyl({ r1, ry, y });
        }
    }

    return mesh(std::move(vertices)
              , std::move(normals)
              , std::move(tex_coords)
              , std::move(indices));
}


mesh frog::gl::shape::cuboid(vec3 size)
{
    auto vertices = std::vector<float>{};
    auto normals = std::vector<float>{};
    auto tex_coords = std::vector<float>{};
    auto indices = std::vector<uint32_t>{};

    using std::vector, std::pair, std::array;
    static const auto side = std::vector<pair<int, int>>
    {
        { -1, -1 },
        {  1, -1 },
        { -1,  1 },
        {  1,  1 },
    };

    static const auto triangles = array<vector<vector<int>>, 2>
    {
        {
            /* one side */
            { { 2, 1, 0 }, { 2, 3, 1 } },

            /* opposite side
             * (reverse order because of face culling) */
            { { 0, 1, 2 }, { 1, 3, 2 } },
        }
    };

    int vert_idx = -1;
    for (int axis = 0; axis < 3; axis++)
    {
        int a = (axis + 1) % 3;
        int b = (axis + 2) % 3;

        for (int sgn : { -1, 1 })
        {
            for (auto[sgn_a, sgn_b] : side)
            {
                vec3 pt;
                pt[a]    = sgn_a * size[a]    / 2;
                pt[b]    = sgn_b * size[b]    / 2;
                pt[axis] = sgn   * size[axis] / 2;

                vec3 n;
                n[a] = 0;
                n[b] = 0;
                n[axis] = sgn;

                // add new vertex
                ++vert_idx;
                vertices.push_back(pt.x());
                vertices.push_back(pt.y());
                vertices.push_back(pt.z());
                normals.push_back(n.x());
                normals.push_back(n.y());
                normals.push_back(n.z());
                tex_coords.push_back((sgn_a + 1) / 2);
                tex_coords.push_back((sgn_b + 1) / 2);
            }
            // add this side's vertices to the indices
            for (auto tri : triangles[(sgn + 1) / 2])
            {
                for (int idx : tri)
                {
                    indices.push_back(vert_idx - 3 + idx);
                }
            }
        }
    }

    // 6 sides, 4 vert each, 3 dimensions
    assert(vertices.size() == 4 * 6 * 3);

    return mesh(std::move(vertices)
              , std::move(normals)
              , std::move(tex_coords)
              , std::move(indices));
}

#endif
