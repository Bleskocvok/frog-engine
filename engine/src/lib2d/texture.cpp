#ifndef NOT_FROG_BUILD_2D

#include "texture.hpp"

#include <stdexcept>    // runtime_error
#include <string>       // ""s

#include "sdl_include.hpp"
#include SDL_HEADER
#include SDL_IMAGE_HEADER


namespace frog::lib2d::gx
{


texture::texture( const lib2d::detail::renderer& renderer, const char* filename )
{
    using namespace std::string_literals;
    lib2d::detail::surface img{ IMG_Load( filename ) };
    if ( !img )
    {
        throw std::runtime_error( ( "texture: Cannot load file '"s )
                                    + filename
                                    + "': "
                                    + IMG_GetError() );
    }
    tex_width = img->w;
    tex_height = img->h;
    tex = lib2d::detail::texture(
            SDL_CreateTextureFromSurface( renderer.get(), img.get() ) );
    if ( !tex )
    {
        throw std::runtime_error( ( "texture: Cannot convert file '"s )
                                    + filename + "' to texture: "
                                    + SDL_GetError() );
    }
}

texture::texture( const lib2d::detail::renderer& renderer,
                  lib2d::detail::surface& img )
{
    using namespace std::string_literals;
    tex_width = img ? img->w : 1;
    tex_height = img ? img->h : 1;
    tex = lib2d::detail::texture( SDL_CreateTextureFromSurface(
                                    renderer.get(), img.get() ) );
    if ( !tex && img )
    {
        throw std::runtime_error(
                    ( "texture: Cannot convert surface to texture: "s )
                    + SDL_GetError() );

    }
}


}  // namespace frog::lib2d::gx


#endif
