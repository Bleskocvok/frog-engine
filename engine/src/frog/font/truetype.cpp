#ifndef NOT_FROG_BUILD_2D

#include "truetype.hpp"

#include "frog/core/engine2d.hpp"
#include "frog/geometry/rectangle.hpp"

#include <utility>      // move

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

void truetype::draw(frog::engine2d& engine, const frog::gx::text& label,
          geo::vec2 pos, float container_height, frog::gx2d::Crop crop)
{
    float height = container_height * label.height;
    auto text_size = size(label.str, height);

    // if (label.centered)
    //     pos.x() -= text_size.x() / 2;

    if (label.align == gx::Align::CENTER)
        pos.x() -= text_size.x() / 2;
    else if (label.align == gx::Align::RIGHT)
        pos.x() -= text_size.x();
    else
        frog_assert(label.align == gx::Align::LEFT);


    pos.x() += text_size.x() / 2;
    geo::rect rect = { pos, text_size };

    auto* cached = texture_cache.get(label.str);
    if (cached)
    {
        engine.draw_sprite(*cached, rect, { 0, 0, 1, 1 }, label.color, crop);
    }
    else
    {
        auto surf = font_.render_text(label.str, 255, 255, 255, 255);
        auto texture = engine.win_raw->make_texture(surf);

        engine.draw_sprite(texture, rect, { 0, 0, 1, 1 }, label.color, crop);

        texture_cache.put(label.str, std::move(texture));
    }
}

// void truetype::draw(frog::engine2d& engine, const std::string& str,
//                     geo::vec2 pos, float height, gx::rgba_t color)
// {
//     pos.x() += size(str, height).x() / 2;

//     auto* cached = texture_cache.get(str);
//     if (cached)
//     {
//         engine.draw_sprite(*cached, { pos, size(str, height) }, { 0, 0, 1, 1 },
//                            color);
//     }
//     else
//     {
//         auto surf = font_.render_text(str, 255, 255, 255, 255);
//         auto texture = engine.win_raw->make_texture(surf);

//         engine.draw_sprite(texture, { pos, size(str, height) }, { 0, 0, 1, 1 },
//                            color);

//         texture_cache.put(str, std::move(texture));
//     }
// }


} // namespace frog

#endif
