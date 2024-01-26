#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "sdl_include.hpp"
#include "os/window_base.hpp"

#include SDL_HEADER
#include SDL_IMAGE_HEADER

#include <stdexcept>    // runtime_error
#include <memory>       // unique_ptr
#include <string>       // string, ""s
#include <cstddef>      // size_t
#include <cstdint>      // uint64_t, int32_t
#include <array>        // array
#include <map>          // map
#include <tuple>        // tuple


namespace frog
{


namespace lib2d
{


namespace detail
{
    struct core_deleter
    {
        // CORE/IMG
        void operator()( SDL_Surface* obj ) { SDL_FreeSurface( obj ); }
        void operator()( SDL_Texture* obj ) { SDL_DestroyTexture( obj ); }
        void operator()( SDL_Window* obj ) { SDL_DestroyWindow( obj ); }
        void operator()( SDL_Renderer* obj ) { SDL_DestroyRenderer( obj ); }
        void operator()( SDL_GameController* c ) { SDL_GameControllerClose( c ); }
        void operator()( SDL_RWops* obj ){ SDL_RWclose( obj ); }
        // // TTF
        // void operator()( TTF_Font* font ) { TTF_CloseFont(font); }
    };

    using surface = std::unique_ptr<SDL_Surface, core_deleter>;
    using texture = std::unique_ptr<SDL_Texture, core_deleter>;
    using window = std::unique_ptr<SDL_Window, core_deleter>;
    using renderer = std::unique_ptr<SDL_Renderer, core_deleter>;
    using game_controller = std::unique_ptr<SDL_GameController, core_deleter>;
    using rwops = std::unique_ptr<SDL_RWops, core_deleter>;

}  // namespace detail


class init_core
{
public:
    init_core( Uint32 flags );
    ~init_core();

    init_core( const init_core& ) = delete;
    init_core& operator=( const init_core& ) = delete;
    init_core( init_core&& ) = default;
    init_core& operator=( init_core&& ) = default;
};


class init_image
{
public:
    init_image( int flags );
    ~init_image();

    init_image( const init_image& ) = delete;
    init_image& operator=( const init_image& ) = delete;
    init_image( init_image&& ) = default;
    init_image& operator=( init_image&& ) = default;
};


// class init_ttf
// {
// public:
//     init_ttf();
//     ~init_ttf();

//     init_ttf( const init_ttf& ) = delete;
//     init_ttf& operator=( const init_ttf& ) = delete;
//     init_ttf( init_ttf&& ) = default;
//     init_ttf& operator=( init_ttf&& ) = default;
// };


namespace fs
{
    enum class mode : bool { Text = false, Binary = true, };

    [[nodiscard]] std::string file_read( const char* filename, mode m );

    void file_write( const char* filename, const std::string& data, mode m );

}  // namespace fs


namespace os
{
    void wait( long ms );

    class timer
    {
        std::uint64_t point = 0;
    public:
        void reset();
        std::uint64_t reset_ms();
        std::uint64_t get_ms() const;

        unsigned duration_us() const;
        unsigned reset_duration_us();
    };

    void error_box( const std::string& title, const std::string& msg );

}  // namespace os


namespace gx
{

enum class Vsync : bool { Off = false, On = true };
enum class Mode : int
{
    Windowed = 0, Borderless = 1, Fullscreen = 2, /* TODO:FullscreenBorderless = 3, */
};


class events
{
public:
    struct key_state { bool pressed, released, down; };
    struct finger { bool pressed=0, released=0;
                    float x=0, y=0, dx=0, dy=0, pressure=0; };
    struct mouse { std::int32_t x, y, xrel, yrel;
                   std::uint32_t state; };

    void k_reset();
    void f_reset();

private:
    bool has_quit = false;

    std::array< key_state, SDL_NUM_SCANCODES > m_keys;

    const Uint8* keyboard_state = nullptr;
    int keyboard_state_count = 0;

    std::map< SDL_FingerID, finger > m_fingers;

public:
    events() = default;

    events( const events& ) = delete;
    events& operator=( const events& ) = delete;

    void update();

    bool quit() const { return has_quit; }

    // keys
    const decltype( m_keys )& keys() const { return m_keys; }
    const key_state& k_at( SDL_Scancode k ) const;

    // keyboard_state
    bool kb_down( SDL_Scancode k ) const;

    // touch
    const decltype( m_fingers )& fingers() const { return m_fingers; }
};


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

    void update_size();

    void clear_color( Uint8 r, Uint8 g, Uint8 b, Uint8 a );

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
            int tex_width, int tex_height, Uint8 r, Uint8 g, Uint8 b, Uint8 a );

    void draw_colored_rotated( const texture& tex, int u, int v,
                               int cut_width, int cut_height, int x, int y,
                               int tex_width, int tex_height,
                               Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                               int pivot_x, int pivot_y, float angle,
                               bool flipped = false );

private:
    lib2d::detail::window win;
    lib2d::detail::renderer renderer;
    int win_width, win_height;

    SDL_Surface* create_surface( int width, int height ) const;
};


}  // namespace gx


} // namespace lib2d


} // namespace frog


#endif
