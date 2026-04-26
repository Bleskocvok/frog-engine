#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "sdl_include.hpp"
#include SDL_HEADER
// #include SDL_TTF_HEADER

#include <memory>       // unique_ptr
#include <string>

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_RWops;

// Sigh…
typedef struct _TTF_Font TTF_Font;

// This one fucks it up. It's a typedef.
// struct SDL_GameController;

namespace frog::lib2d::detail
{
    struct Deleter
    {
        // CORE/IMG
        void operator()( SDL_Surface* obj );
        void operator()( SDL_Texture* obj );
        void operator()( SDL_Window* obj );
        void operator()( SDL_Renderer* obj );
        void operator()( SDL_GameController* c );
        void operator()( SDL_RWops* obj );
        void operator()( SDL_Cursor* obj );
        void operator()( SDL_Sensor* obj );
        // TTF
        void operator()( TTF_Font* font );
    };

    using Surface = std::unique_ptr<SDL_Surface, Deleter>;
    using Texture = std::unique_ptr<SDL_Texture, Deleter>;
    using Window = std::unique_ptr<SDL_Window, Deleter>;
    using Renderer = std::unique_ptr<SDL_Renderer, Deleter>;
    using GameController = std::unique_ptr<SDL_GameController, Deleter>;
    using RWops = std::unique_ptr<SDL_RWops, Deleter>;
    using Cursor = std::unique_ptr<SDL_Cursor, Deleter>;
    using Sensor = std::unique_ptr<SDL_Sensor, Deleter>;

    // TTF
    using Font = std::unique_ptr<TTF_Font, Deleter>;

    Surface load_img( const std::string& filename );

}  // namespace frog::lib2d::detail

#endif
