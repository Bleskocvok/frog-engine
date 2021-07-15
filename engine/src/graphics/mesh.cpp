#include "mesh.hpp"

#include <cassert>


frog::gl::mesh::mesh(std::vector<float> vertices_buf
             , std::vector<float> normals_buf
             , std::vector<float> tex_coords_buf
             , std::vector<uint32_t> indices_buf)
    : vertices(3, vertices_buf)
    , normals(3, normals_buf)
    , tex_coords(2, tex_coords_buf)
    , indices(indices_buf)
{
    assert(vertices_buf.size() == normals_buf.size());
    assert(tex_coords_buf.size() * 3 == normals_buf.size() * 2);

    // assigns to the vao bringing it all together
    vertices.assign(_vao, VerticesLocation);
    normals.assign(_vao, NormalsLocation);
    tex_coords.assign(_vao, TexCoordsLocation);
    indices.assign(_vao);
}


void frog::gl::mesh::draw()
{
    _vao.bind();
    if (indices.good())
    {
        // draw with indices
        glDrawElements(Mode, indices.count(), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        // otherwise draw raw
        glDrawArrays(Mode, 0, vertices.count() / 3);
    }
}

