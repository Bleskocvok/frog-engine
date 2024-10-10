#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "frog/gx3d/include.hpp"
#include "frog/geometry/matrix.hpp"
#include "frog/geometry/vector.hpp"

#include <utility>      // move


namespace frog::gl
{


class shader
{
    GLuint _index = 0;

public:
    shader(GLenum type, const std::string& file);

    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

    shader(shader&& other) noexcept { *this = std::move(other); }
    shader& operator=(shader&& other) noexcept
    {
        _index = other._index;
        other._index = 0;
        return *this;
    }

    ~shader();

    GLuint index() const { return _index; }
};


class program
{
    GLuint _program = 0;

public:
    program(const std::string& vert_file, const std::string& frag_file);

    program(const shader& vert, const shader& frag);

    program(const program&) = delete;
    program& operator=(const program&) = delete;

    program(program&& other) noexcept { *this = std::move(other); }
    program& operator=(program&& other) noexcept
    {
        _program = other._program;
        other._program = 0;
        return *this;
    }

    ~program();

    void use() const;

    GLuint index() const { return _program; }

    void uniform_mat(const geo::mat4& mat, GLint location);
    void uniform_vec(const geo::vec4& vec, GLint location);
    void uniform_vec(const geo::vec3& vec, GLint location);
    void uniform_vec(const geo::vec2& vec, GLint location);
    // TODO: add the other overloads
};



} // namespace frog::gl

#endif
