#pragma once

#include <string>
#include <optional>


namespace frog::gl
{


struct text
{
    std::string str;
    float height = 1;   // relative to its parent's height
    bool centered = false;
};


struct ui_element
{
    std::string sprite;

    std::optional<text> label;

    geo::vec2 pos;
    geo::vec2 size;
    geo::vec2 tex_pos = { 0, 0 };
    geo::vec2 tex_size = { 1, 1 };
    geo::vec4 color = { 1, 1, 1, 1 };
};


} // namespace frog::gl
