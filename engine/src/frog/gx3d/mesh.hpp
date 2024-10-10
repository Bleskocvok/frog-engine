#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "vertex_objects.hpp"
#include "location.hpp"

#include <vector>
#include <cstdint>  // uint32_t


namespace frog::gl
{


class mesh
{
    static constexpr GLenum Mode = GL_TRIANGLES;
    static constexpr GLint VerticesLocation = location::Vertices;
    static constexpr GLint NormalsLocation = location::Normals;
    static constexpr GLint TexCoordsLocation = location::TexCoords;

    vao _vao{};

    vbo vertices;
    vbo normals;
    vbo tex_coords;
    vbo_indices indices;

public:
    mesh(std::vector<float> vertices_buf
        , std::vector<float> normals_buf
        , std::vector<float> tex_coords_buf)
        : mesh(std::move(vertices_buf)
             , std::move(normals_buf)
             , std::move(tex_coords_buf)
             , {})
    {}

    mesh(std::vector<float> vertices_buf
       , std::vector<float> normals_buf
       , std::vector<float> tex_coords_buf
       , std::vector<uint32_t> indices_buf);

    mesh(const mesh&) = delete;
    mesh& operator=(const mesh&) = delete;

    mesh(mesh&&) noexcept = default;
    mesh& operator=(mesh&&) = default;

    ~mesh() = default;

    void draw();
};


} // namespace frog::gl

#endif
