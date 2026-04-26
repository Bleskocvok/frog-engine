#ifndef NOT_FROG_BUILD_2D

#include "structs.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER
#include SDL_IMAGE_HEADER
#include SDL_TTF_HEADER

namespace frog::lib2d::detail
{
    // CORE/IMG
    void Deleter::operator()( SDL_Surface* obj ) { SDL_FreeSurface( obj ); }
    void Deleter::operator()( SDL_Texture* obj ) { SDL_DestroyTexture( obj ); }
    void Deleter::operator()( SDL_Window* obj ) { SDL_DestroyWindow( obj ); }
    void Deleter::operator()( SDL_Renderer* obj ) { SDL_DestroyRenderer( obj ); }
    void Deleter::operator()( SDL_GameController* c ) { SDL_GameControllerClose( c ); }
    void Deleter::operator()( SDL_RWops* obj ){ SDL_RWclose( obj ); }
    void Deleter::operator()( SDL_Cursor* obj ){ SDL_FreeCursor( obj ); }
    void Deleter::operator()( SDL_Sensor* obj ){ SDL_SensorClose( obj ); }
    // TTF
    void Deleter::operator()( TTF_Font* font ) { TTF_CloseFont(font); }

    Surface load_img( const std::string& filename )
    {
        using namespace std::string_literals;
        Surface img{ IMG_Load( filename.c_str() ) };
        if ( not img )
        {
            throw std::runtime_error( ( "load_img: Cannot load file '"s )
                                        + filename
                                        + "': "
                                        + IMG_GetError() );
        }

        return img;
    }

}  // namespace frog::lib2d::detail

#endif
