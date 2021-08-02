#include "error_box.hpp"

#include <SDL.h>

#include <iostream>


void frog::os::show_error_box(const std::string& title,
                              const std::string& message)
{
    int r = SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                title.c_str(),
                message.c_str(),
                nullptr);         // parent window ptr, not needed

    // if message box failed, print error to stderr
    if (r < 0)
    {
        std::cerr << title << ": " << message << std::endl;
    }
}
