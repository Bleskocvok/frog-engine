#pragma once

#include "structs.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER
#include SDL_TTF_HEADER

#include <stdexcept>
#include <string>


namespace frog::lib2d {


class font
{
    detail::font font_;
public:
    font(const std::string& path, int size)
        : font_( TTF_OpenFont( path.c_str(), size ) )
    {
        using namespace std::string_literals;

        if (!font_)
            throw std::runtime_error("cannot load font: "s + TTF_GetError());
    }

    detail::surface render_text(const std::string& text)
    {
        using namespace std::string_literals;

        SDL_Color fg = { 0, 200, 0, 255 };
        auto res = detail::surface{ TTF_RenderText_Blended(font_.get(), text.c_str(), fg) };
        if (!res)
            throw std::runtime_error("cannot render text: "s + TTF_GetError());
        return res;
    }
};


}  // namespace frog::lib2d
