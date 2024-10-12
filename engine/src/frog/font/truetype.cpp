#include "truetype.hpp"

#include "frog/core/engine2d.hpp"


namespace frog::font {


frog::font::truetype::truetype(const std::string& file, int size)
    : font_(file, size)
{ }

geo::vec2 truetype::size(const std::string& str, float height) const
{
    return { height * str.length(), height };
}

void truetype::draw(frog::engine2d& engine, const std::string& str,
                    geo::vec2 pos, float height, gx::rgba_t color)
{
    // TODO: Figure out a good OOP way to make this not horribly inefficient.
    auto surf = font_.render_text(str);
    auto texture = engine.win_raw->make_texture(surf);

    pos.x() += size(str, height).x() / 2;

    engine.draw_sprite(texture, { pos, size(str, height) }, { 0, 0, 1, 1 },
                       gx::colors::white);
}


} // namespace frog
