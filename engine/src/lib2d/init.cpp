#ifndef NOT_FROG_BUILD_2D

#include "core.hpp"

#include SDL_HEADER
#include SDL_IMAGE_HEADER


namespace frog::lib2d
{


init_core::init_core( Uint32 flags )
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


// init_ttf::init_ttf()
// {
//     if ( TTF_Init() != 0 )
//         throw std::runtime_error( TTF_GetError() );
// }

// init_ttf::~init_ttf() { TTF_Quit(); }


}  // namespace frog::lib2d::lib


#endif
