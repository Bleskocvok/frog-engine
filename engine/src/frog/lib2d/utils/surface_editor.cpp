#ifndef NOT_FROG_BUILD_2D

#include "surface_editor.hpp"

namespace frog::lib2d::utils {

SurfaceEditor::SurfaceEditor(frog::lib2d::detail::Surface& surface_)
    : surface(surface_.get())
{
    frog_assert(surface);
    frog_assert(surface->format->BytesPerPixel == BytesPerPixel);

    w = surface->w;
    h = surface->h;

    SDL_LockSurface(surface);
}

SurfaceEditor::~SurfaceEditor()
{
    SDL_UnlockSurface(surface);
}

std::uint32_t* SurfaceEditor::pixel_pos(frog::geo::ivec2 p)
{
    frog_assert(p.x() < w);
    frog_assert(p.x() >= 0);
    frog_assert(p.y() < h);
    frog_assert(p.y() >= 0);

    auto* pixels = static_cast<std::uint8_t*>(surface->pixels);

    auto* pos = pixels + p.y() * surface->pitch + p.x() * BytesPerPixel;
    return reinterpret_cast<uint32_t*>(pos);
}

frog::gx::rgba_t SurfaceEditor::read_pixel(frog::geo::ivec2 p)
{
    auto* ptr = pixel_pos(p);

    frog::gx::rgba_t c;
    SDL_GetRGBA(*ptr, surface->format, &c.r(), &c.g(), &c.b(), &c.a());
    return c;
}

void SurfaceEditor::write_pixel(frog::geo::ivec2 p, frog::gx::rgba_t c)
{
    *pixel_pos(p) = SDL_MapRGBA(surface->format, c.r(), c.g(), c.b(), c.a());
}


} // namespace frog::lib2d::utils

#endif
