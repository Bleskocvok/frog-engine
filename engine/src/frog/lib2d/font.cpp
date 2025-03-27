#ifndef NOT_FROG_BUILD_2D

#include "font.hpp"

#include "sdl_include.hpp"
#include SDL_HEADER
#include SDL_TTF_HEADER

#include <string>           // ""s
#include <stdexcept>        // runtime_error


namespace frog::lib2d {


font::font(const std::string& path, int size)
    : font_( TTF_OpenFont( path.c_str(), size ) )
{
    using namespace std::string_literals;

    if (not font_)
        throw std::runtime_error("cannot load font: "s + TTF_GetError());
}

detail::surface font::render_text(const std::string& text, std::uint8_t r,
                                  std::uint8_t g, std::uint8_t b,
                                  std::uint8_t a)
{
    using namespace std::string_literals;

    SDL_Color fg = { r, g, b, a };
    auto res = detail::surface{ TTF_RenderUTF8_Blended(font_.get(), text.c_str(), fg) };
    if (not res)
        throw std::runtime_error("cannot render text: "s + TTF_GetError());
    return res;
}

detail::surface font::render_text(const std::string& text)
{
    return render_text(text, 255, 255, 255, 255);
}

std::pair<int, int> font::size_text(const std::string& text)
{
    using namespace std::string_literals;

    int w = -1, h = -1;

    if (TTF_SizeUTF8(font_.get(), text.c_str(), &w, &h) == -1)
        throw std::runtime_error("cannot measure text size: "s + TTF_GetError());

    return { w, h };
}

void font::set_outline(int size)
{
    TTF_SetFontOutline(font_.get(), size);
}


} // namespace frog::lib2d

#endif
