#pragma once

#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"
#include "frog/gx2d/sprite.hpp"

#include <string>
#include <optional>


namespace frog::gx
{


struct text
{
    std::string str;
    float height = 1;   // relative to its parent's height
    bool centered = false;
    gx::rgba_t color = gx::colors::white;
};


struct ui_element
{
    frog::gx2d::sprite sprite;

    std::optional<text> label;

    const geo::vec2& pos() const { return sprite.rect.pos; }
          geo::vec2& pos()       { return sprite.rect.pos; }

    const geo::vec2& size() const { return sprite.rect.size; }
          geo::vec2& size()       { return sprite.rect.size; }
};


} // namespace frog::gx
