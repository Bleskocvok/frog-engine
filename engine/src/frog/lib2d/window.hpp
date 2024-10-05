#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "texture.hpp"
#include "structs.hpp"

#include "frog/os/window_base.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER

#include <cstdint>      // uint8_t


namespace frog::lib2d::gx
{
    enum class Vsync : bool { Off = false, On = true };
    enum class Mode : int
    {
        Windowed = 0, Borderless = 1, Fullscreen = 2, /* TODO:FullscreenBorderless = 3, */
    };


    class window : public frog::os::window_base
    {

    public:
        window( int width, int height, const char* title,
                Vsync vsync = Vsync::On, Mode mode = Mode::Windowed,
                bool maximized = false );

        window( const window& ) = delete;
        window& operator=( const window& ) = delete;
        window( window&& ) = default;
        window& operator=( window&& ) = default;

        int w() const { return win_width; }
        int h() const { return win_height; }

        void screenshot( const std::string& filename ) const;

        void update_size( int w = -1, int h = -1 );

        void clear_color( std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a );

        void clear();

        void swap_buffers() override;

        bool should_close() const override { return false; }

        texture make_texture( const char* filename ) const;

        texture make_texture( lib2d::detail::surface& surface ) const;

        void draw( const texture& tex, int u, int v, int cut_width, int cut_height,
                int x, int y, int tex_width, int tex_height );

        void draw( const texture& tex, int x, int y,
                int tex_width, int tex_height );

        void draw_rotated( const texture& tex, int u, int v,
                        int cut_width, int cut_height, int x, int y,
                        int tex_width, int tex_height,
                        int pivot_x, int pivot_y,
                        float angle, bool flipped = false );

        void draw_colored( const texture& tex, int u, int v,
                int cut_width, int cut_height, int x, int y,
                int tex_width, int tex_height,
                std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a );

        // void draw_colored_rotated( const texture& tex, int u, int v,
        //                         int cut_width, int cut_height, int x, int y,
        //                         int tex_width, int tex_height,
        //                         std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a,
        //                         int pivot_x, int pivot_y, float angle,
        //                         bool flipped = false );

        void draw_colored_rotated( const texture& tex, int u, int v,
                                int cut_width, int cut_height,
                                float x, float y, float tex_width, float tex_height,
                                std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a,
                                float pivot_x, float pivot_y, float angle,
                                bool flipped = false );

    private:
        lib2d::detail::window win;
        lib2d::detail::renderer renderer;
        int win_width, win_height;

        SDL_Surface* create_surface( int width, int height ) const;
    };
}

#endif
