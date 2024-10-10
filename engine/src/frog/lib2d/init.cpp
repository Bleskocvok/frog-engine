#ifndef NOT_FROG_BUILD_2D

#include "core.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER
#include SDL_IMAGE_HEADER
#include SDL_TTF_HEADER

#include <cstdint>      // uint32_t
#include <stdexcept>    // runtime_error


namespace frog::lib2d
{


initializer::initializer(std::uint32_t flags)
    : core(flags_to_lib(flags))
    , ttf() { }

std::uint32_t initializer::flags_to_lib(std::uint32_t flags)
{
    std::uint32_t res = 0;
    if (flags & OnlyEvents) res |= SDL_INIT_EVENTS;
    if (flags & Video)      res |= SDL_INIT_VIDEO;
    if (flags & Audio)      res |= SDL_INIT_AUDIO;
    return res;
}


init_core::init_core( std::uint32_t flags )
{
    int ret = SDL_Init( flags );
    if ( ret != 0 )
        throw std::runtime_error( SDL_GetError() );
}

init_core::~init_core() { SDL_Quit(); }


init_image::init_image( int flags )
{
    if ( ( IMG_Init( flags ) & flags ) != flags )
        throw std::runtime_error( IMG_GetError() );
}

init_image::~init_image() { IMG_Quit(); }


init_ttf::init_ttf()
{
    if ( TTF_Init() != 0 )
        throw std::runtime_error( TTF_GetError() );
}

init_ttf::~init_ttf() { TTF_Quit(); }


}  // namespace frog::lib2d::lib


#endif
