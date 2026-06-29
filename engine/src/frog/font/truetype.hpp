#pragma once

#include "base.hpp"
#include "cache.hpp"

#include "frog/lib2d/font.hpp"
#include "frog/lib2d/texture.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <utility>

namespace frog::font {

class truetype : public base
{
    std::map<unsigned, lib2d::font> fonts_;

    struct CacheEntry
    {
        frog::lib2d::gx::texture tex;
        std::uint64_t bytes = 0;
    };

    using CacheKey = std::pair<std::string, unsigned>;

    // TODO: Limit the amount of memory cache can take up.
    static constexpr int CACHE_SIZE = 100;
    Cache<CacheKey, CacheEntry> texture_cache{ CACHE_SIZE };

    unsigned closest_size(unsigned size);

    lib2d::font& closest_font(unsigned size);

public:
    truetype(const std::string& file, int size, int outline = 0);

    frog::geo::vec2 size(const std::string& str, float height, float scale) override;

    // void draw(frog::engine2d&, const std::string& str,
    //           frog::geo::vec2 pos, float height, frog::gx::rgba_t color) override;

    void draw(frog::r2d::Renderer&, const frog::gx::Text& label,
              frog::geo::vec2 pos, float height, float scale,
              frog::gx2d::Crop crop = {}) override;
};

} // namespace frog
