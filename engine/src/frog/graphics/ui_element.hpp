#pragma once

#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"

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
    std::string sprite;

    std::optional<text> label;

    geo::vec2 pos;
    geo::vec2 size = { 1, 1 };
    geo::vec2 tex_pos = { 0, 0 };
    geo::vec2 tex_size = { 1, 1 };
    gx::rgba_t color = gx::colors::white;
};


} // namespace frog::gx
