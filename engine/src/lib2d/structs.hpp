#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "sdl_include.hpp"
#include SDL_HEADER

#include <memory>       // unique_ptr

struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_RWops;

// This one fucks it up. It's a typedef.
// struct SDL_GameController;

namespace frog::lib2d::detail
{
    struct deleter
    {
        // CORE/IMG
        void operator()( SDL_Surface* obj );
        void operator()( SDL_Texture* obj );
        void operator()( SDL_Window* obj );
        void operator()( SDL_Renderer* obj );
        void operator()( SDL_GameController* c );
        void operator()( SDL_RWops* obj );
        // // TTF
        // void operator()( TTF_Font* font );
    };

    using surface = std::unique_ptr<SDL_Surface, deleter>;
    using texture = std::unique_ptr<SDL_Texture, deleter>;
    using window = std::unique_ptr<SDL_Window, deleter>;
    using renderer = std::unique_ptr<SDL_Renderer, deleter>;
    using game_controller = std::unique_ptr<SDL_GameController, deleter>;
    using rwops = std::unique_ptr<SDL_RWops, deleter>;

}  // namespace frog::lib2d::detail

#endif
