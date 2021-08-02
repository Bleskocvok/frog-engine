#pragma once

#include <SDL.h>

#include <type_traits>



namespace frog::os::sdl
{

struct Deleter
{
    void operator()(SDL_Window* win)    { SDL_DestroyWindow(win); }
    void operator()(SDL_GLContext ctx)  { SDL_GL_DeleteContext(ctx); }
};

using window = std::unique_ptr<SDL_Window, Deleter>;
using gl_ctx_noptr = std::remove_pointer_t<SDL_GLContext>;
using gl_context = std::unique_ptr<gl_ctx_noptr, Deleter>;


} // namespace frog::os::sdl



namespace frog::os
{


class window
{
    sdl::window _window;
    sdl::gl_context _context;

public:
    window(int w, int h, const std::string& title)
    {
        _window = std::make_unique<SDL_Window>(
            SDL_CreateWindow(title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            w, h,
            SDL_WINDOW_RESIZABLE
                | SDL_WINDOW_OPENGL));

        if (!_window)
        {

        }

        _context = std::make_unique<gl_ctx_noptr>(
            SDL_GL_CreateContext(_window.get()));

        if (!_context)
        {

        }
    }

    void make_context_current()
    {
        SDL_GL_MakeCurrent(_window.get(), _context.get());
    }

    void swap_buffers()
    {
        SDL_GL_SwapWindow(_window.get());
    }
};


} // namespace frog::os
