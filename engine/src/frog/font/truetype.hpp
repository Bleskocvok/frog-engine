#pragma once

#include "base.hpp"
#include "cache.hpp"

#include "frog/lib2d/font.hpp"
#include "frog/lib2d/texture.hpp"

#include <string>

namespace frog::font {

class truetype : public base
{
    lib2d::font font_;

    // TODO: Limit the amount of memory cache can take up.
    static constexpr int CACHE_SIZE = 1000;
    Cache<std::string, frog::lib2d::gx::texture> texture_cache{ CACHE_SIZE };

public:
    truetype(const std::string& file, int size, int outline = 0);

    frog::geo::vec2 size(const std::string& str, float height) override;

    // void draw(frog::engine2d&, const std::string& str,
    //           frog::geo::vec2 pos, float height, frog::gx::rgba_t color) override;

    void draw(frog::r2d::Renderer&, const frog::gx::Text& label,
              frog::geo::vec2 pos, float height,
              frog::gx2d::Crop crop = {}) override;
};

} // namespace frog
