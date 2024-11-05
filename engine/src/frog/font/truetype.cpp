#ifndef NOT_FROG_BUILD_2D

#include "truetype.hpp"

#include "frog/core/engine2d.hpp"


namespace frog::font {


frog::font::truetype::truetype(const std::string& file, int size, int outline)
    : font_(file, size)
{
    if (outline > 0)
        font_.set_outline(outline);
}

geo::vec2 truetype::size(const std::string& str, float height)
{
    auto[w, h] = font_.size_text(str);
    return { w / float(h) * height, height };
}

void truetype::draw(frog::engine2d& engine, const std::string& str,
                    geo::vec2 pos, float height, gx::rgba_t color)
{
    pos.x() += size(str, height).x() / 2;

    // TODO: Figure out a good OOP way to make this not horribly inefficient.
    auto surf = font_.render_text(str, color.r(), color.g(), color.b(), color.a());
    auto texture = engine.win_raw->make_texture(surf);

    engine.draw_sprite(texture, { pos, size(str, height) }, { 0, 0, 1, 1 },
                       gx::colors::white);
}


} // namespace frog

#endif
