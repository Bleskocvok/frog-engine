#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "texture.hpp"

#include "structs.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER


namespace frog::lib2d::gx
{
    class window;

    class texture
    {
    public:
        texture( const texture& ) = delete;
        texture& operator=( const texture& ) = delete;

        texture( texture&& prev ) = default;
        texture& operator=( texture&& prev ) = default;

        int w() const { return tex_width; }
        int h() const { return tex_height; }
        float ratio() const { return tex_width / static_cast<float>( tex_height ); }

    private:
        friend window;

        lib2d::detail::texture tex;
        int tex_width, tex_height;

        texture( const lib2d::detail::renderer& renderer, const char* filename );

        texture( const lib2d::detail::renderer& renderer, lib2d::detail::surface& img );

        SDL_Texture* src() const { return tex.get(); }
    };
}

#endif
