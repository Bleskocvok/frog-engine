#pragma once

#include "base.hpp"


namespace frog::font {


class atlas : public base
{
    static constexpr int grid_size = 16;
    static constexpr float size_ratio = 1;
    static constexpr float spacing = 0.75;

    lib2d::gx::texture texture;

    frog::geo::vec2 char_size(float height) const;

public:
    atlas(frog::engine2d&, const std::string& atlas_file,
          const std::string& config_file);

    frog::geo::vec2 size(const std::string& str, float height) override;

    void draw(frog::engine2d&, const std::string& str,
              frog::geo::vec2 pos, float height, frog::gx::rgba_t color) override;
};


} // namespace frog
