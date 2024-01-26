#ifndef NOT_FROG_BUILD_3D

#include "mesh.hpp"

#include "tiny_obj_loader.h"

#include <cassert>
#include <stdexcept>


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
    if (indices)
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

// TODO finish this
static void load_file(const std::string& filename)
{
    tinyobj::attrib_t attr;
    std::vector<tinyobj::shape_t> shapes;
    std::string errors;
    bool ok = tinyobj::LoadObj(
        &attr,
        &shapes,
        nullptr,            // materials (not needed)
        &errors,
        filename.c_str(),
        nullptr,            // folder for materials (not needed)
        true                // triangulate = true
    );

    if (!ok)
    {
        throw std::runtime_error("could not load model, file '" + filename
                                 + "'; errors: " + errors);
    }

    //Meshes result;
    for (const auto& shape : shapes)
    {
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> tex_coords;

        size_t size = shape.mesh.num_face_vertices.size();
        size_t triangle = 3;  // a dumb constant to make the code more readable
                              // 3 == number of points in a triangle

        vertices.reserve(size * triangle * 3);
        normals.reserve(size * triangle * 3);
        tex_coords.reserve(size * triangle * 2);

        for (size_t i = 0; i < size; ++i)
        {
            for (size_t j = 0; j < triangle; ++j)
            {
                tinyobj::index_t index = shape.mesh.indices[i * 3 + j];
                // vertices
                vertices.push_back(attr.vertices[3 * index.vertex_index + 0]);
                vertices.push_back(attr.vertices[3 * index.vertex_index + 1]);
                vertices.push_back(attr.vertices[3 * index.vertex_index + 2]);
                // normals
                normals.push_back(attr.normals[3 * index.normal_index + 0]);
                normals.push_back(attr.normals[3 * index.normal_index + 1]);
                normals.push_back(attr.normals[3 * index.normal_index + 2]);
                // texture coords
                tex_coords.push_back(attr.texcoords[2 * index.texcoord_index + 0]);
                tex_coords.push_back(attr.texcoords[2 * index.texcoord_index + 1]);
            }
        }

        //result.push_back(
            //Mesh(vertices, normals, tex_coords, std::vector<uint32_t>{}));
    }
    //return result;
}

#endif
