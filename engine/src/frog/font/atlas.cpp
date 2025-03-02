#ifndef NOT_FROG_BUILD_2D

#include "frog/core/engine2d.hpp"

#include "atlas.hpp"

#include <iterator>         // distance
#include <algorithm>        // find


namespace frog::font {


frog::font::atlas::atlas(frog::engine2d& engine, const std::string& atlas_file,
                         const std::string& /* config_file */)
    : texture(engine.win_raw->make_texture(atlas_file.c_str()))
{ }

frog::geo::vec2 atlas::char_size(float height) const
{
    return { height * spacing * size_ratio, height };
}

geo::vec2 atlas::size(const std::string& str, float height)
{
    return { str.length() * height
                          * spacing
                          * size_ratio
           , height };
}

void atlas::draw(frog::engine2d& engine, const std::string& str,
                 geo::vec2 pos, float height, gx::rgba_t color)
{
    using namespace frog;
    using namespace frog::geo;

    // TODO: Load this from config_file
    static const std::string map = "ABCDEFGHIJKLMNOP"
                                   "QRSTUVWXYZ012345"
                                   "6789 .,!?-:/_'#*"
                                   "\"+abcdefghijklmn"
                                   "opqrstuvwxyz=() "
                                   "$%&<>@[]^`|{}\\";

    pos.x() += char_size(height).x() / 2;

    for (char c : str)
    {
        size_t idx = std::distance(map.begin(),
                                   std::find(map.begin(), map.end(), c));

        if (idx >= map.size())
        {
            pos.x() += char_size(height).x();
            continue;
        }

        geo::vec2 tex_size = { 1.f / grid_size,
                               size_ratio / grid_size };

        geo::vec2 tex_pos = { static_cast<float>(idx % grid_size),
                              static_cast<float>(idx / grid_size) };

        tex_pos *= tex_size;

        engine.draw_sprite(texture, { pos, vec2{ height } }, { tex_pos, tex_size }, color);
        pos.x() += char_size(height).x();
    }
}


} // namespace frog

#endif
