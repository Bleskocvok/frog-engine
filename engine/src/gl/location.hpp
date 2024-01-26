#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "gl/include.hpp"


namespace frog::gl::location
{


struct material
{
    GLint ambient{};
    GLint diffuse{};
    GLint specular{};
};


static constexpr GLuint TextureBinding  = 0;

static constexpr GLint Vertices         = 0;
static constexpr GLint Normals          = 1;
static constexpr GLint TexCoords        = 2;

static constexpr GLint ProjectionMatrix = 0;
static constexpr GLint ViewMatrix       = 1;
static constexpr GLint ModelMatrix      = 2;

static constexpr material Material      { 3, 4, 5 };
static constexpr material Light         { 6, 7, 8 };

static constexpr GLint LightPos         = 9;

static constexpr GLint EyePos           = 10;


namespace ui
{


static constexpr GLint Pos              = 0;
static constexpr GLint Size             = 1;
static constexpr GLint TexPos           = 2;
static constexpr GLint TexSize          = 3;
static constexpr GLint TexColor         = 4;


} // namespace ui


} // namespace frog


#endif
