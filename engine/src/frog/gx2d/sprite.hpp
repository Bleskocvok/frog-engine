#pragma once

#include "frog/geometry/rectangle.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"
#include "crop.hpp"

#include <optional>
#include <string>


namespace frog::gx2d {

struct prev_sprite
{
    geo::vec2 pos;
    float angle = 0;
};

enum class Interpolation
{
    NONE,
    INTERPOLATE,
    EXTRAPOLATE
};

struct sprite
{
    std::string image_tag;
    geo::rect rect;

    gx::rgba_t color = gx::colors::white;
    float angle = 0;
    bool flipped = false;

    geo::rect tex = { 0, 0, 1, 1 };

    // lower layer = rendered earlier
    unsigned layer = 0;

    prev_sprite prev;
    Interpolation interpolation = Interpolation::NONE;

    std::optional<Crop> crop = std::nullopt;
};


inline void calculate_prev(sprite& sprite)
{
    sprite.prev.pos = sprite.rect.pos;
    sprite.prev.angle = sprite.angle;
}

inline void crop_rect(const Crop& crop, geo::rect& rect)
{
    // TODO: Left and right.
    rect.size.y() -= crop.top + crop.bot;
    rect.pos.y() -= crop.bot * 0.5 - crop.top * 0.5;
}

inline void crop_tex(const Crop& crop, const geo::rect& rect, geo::rect& tex)
{
    // TODO: Left and right.
    float top_ratio = crop.top / rect.size.y();
    float top_remove = top_ratio * tex.size.y();

    float bot_ratio = crop.bot / rect.size.y();
    float bot_remove = bot_ratio * tex.size.y();

    tex.size.y() -= top_remove;
    tex.pos.y() += top_remove;

    tex.size.y() -= bot_remove;
}


}  // namespace frog::2d
