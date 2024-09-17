#ifndef NOT_FROG_BUILD_2D

#include "core.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER

#include <iostream>     // cerr


namespace frog::lib2d
{


void os::error_box( const std::string& title, const std::string& msg )
{
    if ( SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, title.c_str(),
                                   msg.c_str(), nullptr ) < 0 )
    {
        std::cerr << "ERROR-ERROR: cannot show error message: '"
                  << title << ": " << msg << "'" << std::endl;
    }
}


} // namespace frog

#endif
