#ifndef NOT_FROG_BUILD_2D

#include "window.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER

#include <stdexcept>    // runtime_error
#include <string>       // ""s

#include "frog/utils/debug.hpp"
namespace frog::lib2d::gx
{


window::window( int width, int height, const char* title,
        Vsync vsync, Mode mode, bool maximized )
        : win_width( width ), win_height( height )
{
    using namespace std::string_literals;

    std::uint32_t win_flags = SDL_WINDOW_RESIZABLE;
    std::uint32_t ren_flags = SDL_RENDERER_ACCELERATED;
    std::uint32_t pos = SDL_WINDOWPOS_UNDEFINED;

    if ( vsync == Vsync::On )
        ren_flags = ren_flags | SDL_RENDERER_PRESENTVSYNC;

    if ( maximized )
        win_flags = win_flags | SDL_WINDOW_MAXIMIZED;

    if ( mode == Mode::Windowed )
    {
        win_flags = win_flags;
    }
    else if ( mode == Mode::Borderless )
    {
        pos = SDL_WINDOWPOS_CENTERED;
        win_flags = win_flags | SDL_WINDOW_BORDERLESS;
        // SDL_DisplayMode dm;
        // int ret = SDL_GetDesktopDisplayMode( &dm );
        // width =
        // if ( ret != 0 ) {
        //     // error
        // }
        // TODO: ^^
    }
    else if ( mode == Mode::Fullscreen )
    {
        win_flags = win_flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    win = lib2d::detail::window( SDL_CreateWindow( title,
                                                 pos, // pos x
                                                 pos, // pos y
                                                 width,
                                                 height,
                                                 win_flags ) );
    if ( !win )
        throw std::runtime_error( "Create window: "s += SDL_GetError() );

    renderer = lib2d::detail::renderer( SDL_CreateRenderer( win.get(), -1,
                                                          ren_flags ) );
    if ( !renderer )
        throw std::runtime_error( "Create Renderer: "s
                                    += SDL_GetError() );

    // TODO: move somewhere else and give the option to use linear instead
    // nearest pixel sampling
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" );
}


void window::update_size( int w, int h )
{
    if ( w == -1 || h == -1 )
    {
        SDL_GetWindowSize( win.get(), &win_width, &win_height );
    }
    else
    {
        win_width = w;
        win_height = h;
    }
}


void window::clear_color( std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a )
{
    SDL_SetRenderDrawColor( renderer.get(), r, g, b, a );
}


void window::clear()
{
    SDL_RenderClear( renderer.get() );
}


void window::swap_buffers()
{
    SDL_RenderPresent( renderer.get() );
}


void window::set_icon( lib2d::detail::surface& surface )
{
    SDL_SetWindowIcon( win.get(), surface.get() );
}


texture window::make_texture( const char* filename ) const
{
    return texture( renderer, filename );
}


texture window::make_texture( lib2d::detail::surface& surface ) const
{
    return texture( renderer, surface );
}


void window::draw( const texture& tex, int u, int v,
        int cut_width, int cut_height,
        int x, int y, int tex_width, int tex_height )
{
    if ( !tex.src() ) throw std::runtime_error( "draw empty texture" );

    SDL_Rect src = { u, v, cut_width, cut_height };
    SDL_Rect dest = { x, y, tex_width, tex_height };
    SDL_RenderCopy( renderer.get(), tex.src(), &src, &dest );
}


void window::draw( const texture& tex, int x, int y,
            int tex_width, int tex_height )
{
    if ( !tex.src() ) throw std::runtime_error( "draw empty texture" );

    SDL_Rect dest = { x, y, tex_width, tex_height };
    SDL_RenderCopy( renderer.get(), tex.src(), nullptr, &dest );
}


void window::draw_rotated( const texture& tex, int u, int v,
                    int cut_width, int cut_height, int x, int y,
                    int tex_width, int tex_height,
                    int pivot_x, int pivot_y,
                    float angle, bool flipped )
{
    if ( !tex.src() ) throw std::runtime_error( "draw empty texture" );

    SDL_Rect src = { u, v, cut_width, cut_height };
    SDL_Rect dest = { x, y, tex_width, tex_height };
    SDL_Point center = { pivot_x, pivot_y };
    SDL_RenderCopyEx( renderer.get(),
                        tex.src(),
                        &src,
                        &dest,
                        angle,
                        &center,
                        flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE );
}

void window::draw_colored( const texture& tex, int u, int v,
                           int cut_width, int cut_height, int x, int y,
                           int tex_width, int tex_height,
                           std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a )
{
    if ( !tex.src() ) throw std::runtime_error( "draw empty texture" );

    SDL_SetTextureColorMod( tex.src(), r, g, b );
    SDL_SetTextureAlphaMod( tex.src(), a );

    SDL_Rect src = { u, v, cut_width, cut_height };
    SDL_Rect dest = { x, y, tex_width, tex_height };
    // TODO: Change all of these to ...F version (which use float and render better).
    SDL_RenderCopy( renderer.get(), tex.src(), &src, &dest );

    SDL_SetTextureAlphaMod( tex.src(), 255 );
    SDL_SetTextureColorMod( tex.src(), 255, 255, 255 );
}


// void window::draw_colored_rotated( const texture& tex, int u, int v,
//                             int cut_width, int cut_height, int x, int y,
//                             int tex_width, int tex_height,
//                             std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a,
//                             int pivot_x, int pivot_y, float angle,
//                             bool flipped )
// {
//     if ( !tex.src() ) throw std::runtime_error( "draw empty texture" );

//     SDL_SetTextureColorMod( tex.src(), r, g, b );
//     SDL_SetTextureAlphaMod( tex.src(), a );

//     SDL_Rect src = { u, v, cut_width, cut_height };
//     SDL_Rect dest = { x, y, tex_width, tex_height };
//     SDL_Point center = { pivot_x, pivot_y };
//     SDL_RenderCopyEx( renderer.get(),
//                         tex.src(),
//                         &src,
//                         &dest,
//                         angle,
//                         &center,
//                         flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE );

//     SDL_SetTextureAlphaMod( tex.src(), 255 );
//     SDL_SetTextureColorMod( tex.src(), 255, 255, 255 );
// }


void window::draw_colored_rotated( const texture& tex, int u, int v,
                            int cut_width, int cut_height, float x, float y,
                            float tex_width, float tex_height,
                            std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a,
                            float pivot_x, float pivot_y, float angle,
                            bool flipped )
{
    if ( !tex.src() ) throw std::runtime_error( "draw empty texture" );

    SDL_SetTextureColorMod( tex.src(), r, g, b );
    SDL_SetTextureAlphaMod( tex.src(), a );

    SDL_Rect src = { u, v, cut_width, cut_height };
    SDL_FRect dest = { x, y, tex_width, tex_height };
    SDL_FPoint center = { pivot_x, pivot_y };
    SDL_RenderCopyExF( renderer.get(),
                        tex.src(),
                        &src,
                        &dest,
                        angle,
                        &center,
                        flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE );

    SDL_SetTextureAlphaMod( tex.src(), 255 );
    SDL_SetTextureColorMod( tex.src(), 255, 255, 255 );
}


void window::screenshot( const std::string& filename ) const
{
    using namespace std::string_literals;
    int w, h;
    SDL_GetRendererOutputSize( renderer.get(), &w, &h );
    lib2d::detail::surface surface( create_surface( w, h ) );
    if ( SDL_RenderReadPixels( renderer.get(),
                                nullptr,
                                surface->format->format,
                                surface->pixels,
                                surface->pitch ) != 0 )
    {
        throw std::runtime_error( "could not capture screenshot: "s
                                    + SDL_GetError() );
    }

    if ( SDL_SaveBMP( surface.get(), filename.c_str() ) != 0 )
        throw std::runtime_error( "could not save screenshot: "s
                                    + SDL_GetError() );
}


SDL_Surface* window::create_surface( int width, int height ) const
{
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        return SDL_CreateRGBSurface( 0, width, height, 32,
                0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );
    #else
        return SDL_CreateRGBSurface( 0, width, height, 32,
                0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 );
    #endif
}


}  // namespace frog::lib2d::gx


#endif
