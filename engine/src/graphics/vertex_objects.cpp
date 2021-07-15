#include "vertex_objects.hpp"


frog::gl::vbo::vbo(int dimension, const std::vector<float>& data)
        : dimension(dimension)
{
    if (data.empty())
        return;

    glCreateBuffers(1, &buffer);
    glNamedBufferStorage(
            buffer,
            data.size() * sizeof(float),
            data.data(),
            GL_DYNAMIC_STORAGE_BIT);

    _count = data.size();
}


void frog::gl::vbo::assign(vao& _vao, GLint location)
{
    glVertexArrayVertexBuffer(
            _vao.get(),
            location,
            buffer,
            0,                              // first element
            dimension * sizeof(float));     // size of one whole element
    glEnableVertexArrayAttrib(
            _vao.get(),
            location);
    glVertexArrayAttribFormat(
            _vao.get(),
            location,
            dimension,
            GL_FLOAT,
            GL_FALSE,                       // if normalize
            0);                             // offset to first element
    glVertexArrayAttribBinding(
            _vao.get(),
            location,
            location);
}


frog::gl::vbo_indices::vbo_indices(const std::vector<uint32_t>& data)
{
    if (data.empty())
        return;

    glCreateBuffers(1, &buffer);
    glNamedBufferStorage(
            buffer,
            data.size() * sizeof(uint32_t),
            data.data(),
            GL_DYNAMIC_STORAGE_BIT);

    _count = data.size();
}


void frog::gl::vbo_indices::assign(vao& _vao)
{
    glVertexArrayElementBuffer(_vao.get(), buffer);
}
