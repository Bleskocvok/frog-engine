#pragma once

#include "frog/geometry/rectangle.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"
#include "frog/geometry/general.hpp"
#include "crop.hpp"

#include <cmath>        // lerp
#include <cstdint>      // int16_t, INT16_MIN, INT16_MAX
#include <optional>
#include <string>
#include <vector>


namespace frog::gx2d {

struct prev_sprite
{
    geo::vec2 pos;
    geo::vec2 size;
    std::optional<Crop> crop;
    float angle = 0;
};

enum class Interpolation
{
    NONE,
    INTERPOLATE,
    EXTRAPOLATE
};

struct ChildSprite;

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

    std::vector<ChildSprite> children;
};

constexpr std::int16_t BELOW = INT16_MIN;
constexpr std::int16_t ABOVE = INT16_MAX;

struct Anchor
{
    enum class Position { NONE, RELATIVE, SIZE_RELATIVE };
    Position position = Position::NONE;
    bool rel_size = false;
};

struct ChildSprite
{
    std::int16_t layer = ABOVE;
    Anchor anchor;
    sprite sprite;
};

inline void calculate_prev(sprite& sprite)
{
    sprite.prev.pos = sprite.rect.pos;
    sprite.prev.size = sprite.rect.size;
    sprite.prev.angle = sprite.angle;
    sprite.prev.crop = sprite.crop;
}

inline void crop_rect(const Crop& crop, geo::rect& rect)
{
    // TODO: Left and right.
    rect.size.y() -= crop.top + crop.bot;
    rect.pos.y() -= crop.bot * 0.5 - crop.top * 0.5;

    rect.size.x() -= crop.left + crop.right;
    rect.pos.x() -= crop.right * 0.5 - crop.left * 0.5;
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

inline void perform_interpolation(const sprite& s, double between, geo::rect& rect, float& angle)
{
    if (s.interpolation == gx2d::Interpolation::INTERPOLATE)
    {
        rect.pos = frog::geo::lerp(s.prev.pos, s.rect.pos, float(between));
        rect.size = frog::geo::lerp(s.prev.size, s.rect.size, float(between));
        angle = geo::lerp_deg(s.prev.angle, s.angle, between);
    }
    else if (s.interpolation == gx2d::Interpolation::EXTRAPOLATE)
    {
        rect.pos = frog::geo::lerp(s.prev.pos, s.rect.pos, float(1 + between));
        rect.size = frog::geo::lerp(s.prev.size, s.rect.size, float(1 + between));
        angle = geo::lerp_deg(s.prev.angle, s.angle, 1 + between);
    }
}


inline void apply_crop(const gx2d::sprite& model, double between, geo::rect& rect, geo::rect& tex)
{
    if (not model.crop)
        return;

    auto crop = *model.crop;

    if (model.interpolation != gx2d::Interpolation::NONE)
    {
        float value = float(between);
        if (model.interpolation == gx2d::Interpolation::EXTRAPOLATE)
            value += 1;

        Crop prev = model.prev.crop.value_or(Crop{});

        crop.top = std::lerp(prev.top, crop.top, value);
        crop.bot = std::lerp(prev.bot, crop.bot, value);
        crop.left = std::lerp(prev.left, crop.left, value);
        crop.right = std::lerp(prev.right, crop.right, value);
    }

    crop_tex(crop, rect, tex);
    crop_rect(crop, rect);
}

}  // namespace frog::2d
