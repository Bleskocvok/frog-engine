#ifndef NOT_FROG_BUILD_2D

#include "core.hpp"

#include "structs.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER

#include <stdexcept>    // runtime_error
#include <string>       // string, ""s


namespace frog::lib2d::fs
{


std::string file_read( const char* filename, mode m )
{
    using namespace std::string_literals;

    lib2d::detail::rwops rw( SDL_RWFromFile( filename, bool(m) ? "rb" : "r" ) );

    if ( not rw )
        throw std::runtime_error( "file_read: open: "s + "'" + filename + "':"
                                  + SDL_GetError() );

    auto data = std::string{};

    // necessary because SDL_RWread returns 0 for both an error and EOF
    auto size = SDL_RWsize( rw.get() );
    if ( size < 0 )
        throw std::runtime_error( "file_read: size: "s + "'" + filename + "':"
                                  + SDL_GetError() );

    data.resize( size );

    auto read = decltype( size ){ 0 };
    auto total = decltype( size ){ 0 };
    do
    {
        read = SDL_RWread( rw.get(), data.data() + total, sizeof( char ),
                           size - total );
        total += read;

    } while ( read != 0 && size > total );

    if ( total != size )
        throw std::runtime_error( "file_read: read: "s + "'" + filename + "':"
                                  + SDL_GetError() );

    return data;
}


void file_write( const char* filename, const std::string& data, mode m )
{
    using namespace std::string_literals;

    lib2d::detail::rwops rw( SDL_RWFromFile( filename, bool(m) ? "wb" : "w" ) );

    if ( not rw )
        throw std::runtime_error( "file_write: open: "s + "'" + filename + "':"
                                  + SDL_GetError() );

    auto written = SDL_RWwrite( rw.get(), data.data(), sizeof( char ),
                                data.size() );
    if ( written < data.size() )
        throw std::runtime_error( "file_write: write: "s + "'" + filename + "':"
                                  + SDL_GetError() );
}


}  // namespace frog::lib2d::os


#endif
