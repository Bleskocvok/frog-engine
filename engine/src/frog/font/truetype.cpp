#ifndef NOT_FROG_BUILD_2D

#include "truetype.hpp"

#include "frog/core/engine2d.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/core/2d/renderer.hpp"
#include "frog/utils/assert.hpp"

#include <utility>      // move
#include <cstdint>

namespace frog::font {

unsigned truetype::closest_size(unsigned size)
{
    frog_assert(not fonts_.empty());

    int n = size;
    int match = fonts_.begin()->first;

    for (const auto& [k, v] : fonts_)
    {
        if (std::abs(int(n - k)) < std::abs(n - match))
            match = k;
    }

    return match;
}

lib2d::font& truetype::closest_font(unsigned size)
{
    return fonts_.at( closest_size( size ) );
}

frog::font::truetype::truetype(const std::string& file, int size, int outline)
{
    const unsigned min_size = 8;

    for (unsigned n = min_size; n <= std::max<unsigned>(min_size, size); n *= 2)
    {
        auto [it, ok] = fonts_.emplace(n, lib2d::font(file, n));

        if (outline > 0)
            it->second.set_outline(outline);
    }
}

geo::vec2 truetype::size(const std::string& str, float height, float scale)
{
    unsigned pixels = height * scale;
    auto& font_ = fonts_.at( closest_size( pixels ) );

    auto[w, h] = font_.size_text(str);
    return { w / float(h) * height, height };
}

void truetype::draw(frog::r2d::Renderer& engine, const frog::gx::Text& label,
          geo::vec2 pos, float height, float scale, frog::gx2d::Crop crop)
{
    if (label.str.empty())
        return;

    auto text_size = size(label.str, height, scale);

    pos.x() += text_size.x() / 2;
    geo::rect rect = { pos, text_size };

    std::uint64_t total = 0;
    for (const auto& elem : texture_cache)
        total += elem.val.bytes;
    // LOGX(texture_cache.size(), total);

    unsigned pixels = height * scale;
    unsigned closest = closest_size( pixels );
    auto key = std::make_pair(label.str, closest);

    auto* cached = texture_cache.get(key);
    if (cached)
    {
        engine.draw_ui_sprite(cached->tex, rect, { 0, 0, 1, 1 }, label.color, crop);
    }
    else
    {
        auto& font_ = fonts_.at( closest );

        auto surf = font_.render_text(label.str, label.color.r(), label.color.g(),
                                      label.color.b(), label.color.a());
        std::uint64_t bytes = surf ? surf->pitch * surf->h : 0;
        auto texture = engine.window->make_texture(surf);

        engine.draw_ui_sprite(texture, rect, { 0, 0, 1, 1 }, label.color, crop);

        auto entry = CacheEntry{ .tex = std::move(texture), .bytes = bytes };
        texture_cache.put(key, std::move(entry));
    }
}

} // namespace frog

#endif
