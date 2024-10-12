#pragma once

#include "base.hpp"

#include "frog/lib2d/font.hpp"

#include <string>


namespace frog::font {


class truetype : public base
{
    lib2d::font font_;
public:
    truetype(const std::string& file, int size);

    frog::geo::vec2 size(const std::string& str, float height) override;

    void draw(frog::engine2d&, const std::string& str,
              frog::geo::vec2 pos, float height, frog::gx::rgba_t color) override;
};


} // namespace frog
