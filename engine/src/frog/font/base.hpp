#pragma once

#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"
#include "frog/graphics/ui_element.hpp"

#include <string>


// TODO: Solve issues that arise with this (engine2d).
namespace frog { class engine2d; } // namespace frog

namespace frog::r2d { class Renderer; }

namespace frog::font {


struct base
{
    virtual ~base() = default;

    virtual geo::vec2 size(const std::string& str, float height) = 0;

    virtual void draw(frog::r2d::Renderer&, const frog::gx::text& label,
                      geo::vec2 pos, float height,
                      frog::gx2d::Crop crop = {}) = 0;

    // virtual void draw(frog::engine2d&, const std::string& str,
    //                   frog::geo::vec2 pos, float height, frog::gx::rgba_t color) = 0;
};


} // namespace frog
