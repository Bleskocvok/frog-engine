#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "frog/gl/include.hpp"

#include <string>
#include <utility>          // move


namespace frog::gl
{


class texture
{
    GLuint _index = 0;

public:
    texture(const std::string& filename);

    texture(const texture&) = delete;
    texture& operator=(const texture&) = delete;

    texture(texture&& other) noexcept
    {
        *this = std::move(other);
    }

    texture& operator=(texture&& other) noexcept
    {
        _index = other._index;
        other._index = 0;
        return *this;
    }

    ~texture();

    void bind(GLuint idx);

    GLuint index() const { return _index; }
};


} // namespace frog::gl

#endif
