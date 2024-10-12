#pragma once

#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"

#include <string>


// TODO: Solve issues that arise with this (engine2d).
namespace frog { class engine2d; } // namespace frog


namespace frog::font {


struct base
{
    virtual ~base() = default;

    virtual geo::vec2 size(const std::string& str, float height) const = 0;

    virtual void draw(frog::engine2d&, const std::string& str,
                      frog::geo::vec2 pos, float height, frog::gx::rgba_t color) = 0;
};


} // namespace frog
