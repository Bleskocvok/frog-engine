#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "frog/gx3d/include.hpp"

#include <vector>


namespace frog::gl
{


class vao
{
    GLuint buffer = 0;

public:
    vao()
    {
        glCreateVertexArrays(1, &buffer);
    }

    ~vao()
    {
        glDeleteVertexArrays(1, &buffer);
    }

    vao(const vao&) = delete;
    vao& operator=(const vao&) = delete;

    vao(vao&& other) noexcept
        : buffer(other.buffer)
    {
        other.buffer = 0;
    }

    vao& operator=(vao&& other) noexcept
    {
        buffer = other.buffer;
        other.buffer = 0;
        return *this;
    }

    GLuint get()
    {
        return buffer;
    }

    void bind()
    {
        glBindVertexArray(buffer);
    }
};


class vbo
{
    GLuint buffer = 0;
    int _count = 0;
    int dimension = 0;

public:
    vbo(int dimension, const std::vector<float>& data);

    vbo(const vbo&) = delete;
    vbo& operator=(const vbo&) = delete;

    vbo(vbo&& other) noexcept
        : buffer(other.buffer)
        , _count(other._count)
    {
        other.buffer = 0;
        other._count = 0;
    }

    vbo& operator=(vbo&& other) noexcept
    {
        buffer = other.buffer;
        _count = other._count;
        other.buffer = 0;
        other._count = 0;
        return *this;
    }

    ~vbo()
    {
        glDeleteBuffers(1, &buffer);
    }

    void assign(vao& _vao, GLint location);

    int count() const { return _count; }
};


class vbo_indices
{
    GLuint buffer = 0;
    int _count = 0;

public:
    vbo_indices()
        : vbo_indices(std::vector<uint32_t>{}) {}

    vbo_indices(const std::vector<uint32_t>& data);

    vbo_indices(const vbo_indices&) = delete;
    vbo_indices& operator=(const vbo_indices&) = delete;

    vbo_indices(vbo_indices&& other) noexcept
        : buffer(other.buffer)
        , _count(other._count)
    {
        other.buffer = 0;
        other._count = 0;
    }

    vbo_indices& operator=(vbo_indices&& other) noexcept
    {
        buffer = other.buffer;
        _count = other._count;
        other.buffer = 0;
        other._count = 0;
        return *this;
    }

    ~vbo_indices()
    {
        glDeleteBuffers(1, &buffer);
    }

    void assign(vao& _vao);

    bool good() const
    {
        return buffer > 0;
    }

    operator bool() const
    {
        return good();
    }

    int count() const { return _count; }
};


} // namespace frog::gl


#endif
