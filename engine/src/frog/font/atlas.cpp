#ifndef NOT_FROG_BUILD_2D

#include "frog/core/engine2d.hpp"
#include "frog/core/2d/renderer.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/gx2d/crop.hpp"
#include "frog/gx2d/sprite.hpp"

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

void atlas::draw(frog::r2d::Renderer& renderer, const frog::gx::text& label,
          geo::vec2 pos, float container_height, frog::gx2d::Crop crop)
{
    using namespace frog;
    using namespace frog::geo;

    float height = container_height * label.height;
    auto text_size = size(label.str, height);

    geo::rect full_rect = { pos, text_size };

    if (label.align == gx::Align::CENTER)
        pos.x() -= text_size.x() / 2;
    else if (label.align == gx::Align::RIGHT)
        pos.x() -= text_size.x();
    else
        frog_assert(label.align == gx::Align::LEFT);

    // TODO: Load this from config_file
    static const std::string map = "ABCDEFGHIJKLMNOP"
                                   "QRSTUVWXYZ012345"
                                   "6789 .,!?-:/_'#*"
                                   "\"+abcdefghijklmn"
                                   "opqrstuvwxyz=() "
                                   "$%&<>@[]^`|{}\\";

    pos.x() += char_size(height).x() / 2;

    for (char c : label.str)
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
                              static_cast<float>(decltype(idx)(idx / grid_size)) };

        tex_pos *= tex_size;

        geo::rect tex = { tex_pos, tex_size };
        geo::rect char_rect = { pos, vec2{ height } };

        // Cropping stuff.
        crop = gx2d::multiply_crop(full_rect, crop, char_rect);
        gx2d::crop_tex(crop, char_rect, tex);
        gx2d::crop_rect(crop, char_rect);

        renderer.draw_sprite(texture, char_rect, tex, label.color);
        pos.x() += char_size(height).x();
    }
}

// void atlas::draw(frog::engine2d& engine, const std::string& str,
//                  geo::vec2 pos, float height, gx::rgba_t color)
// {
//     using namespace frog;
//     using namespace frog::geo;

//     // TODO: Load this from config_file
//     static const std::string map = "ABCDEFGHIJKLMNOP"
//                                    "QRSTUVWXYZ012345"
//                                    "6789 .,!?-:/_'#*"
//                                    "\"+abcdefghijklmn"
//                                    "opqrstuvwxyz=() "
//                                    "$%&<>@[]^`|{}\\";

//     pos.x() += char_size(height).x() / 2;

//     for (char c : str)
//     {
//         size_t idx = std::distance(map.begin(),
//                                    std::find(map.begin(), map.end(), c));

//         if (idx >= map.size())
//         {
//             pos.x() += char_size(height).x();
//             continue;
//         }

//         geo::vec2 tex_size = { 1.f / grid_size,
//                                size_ratio / grid_size };

//         geo::vec2 tex_pos = { static_cast<float>(idx % grid_size),
//                               static_cast<float>(idx / grid_size) };

//         tex_pos *= tex_size;

//         engine.draw_sprite(texture, { pos, vec2{ height } }, { tex_pos, tex_size }, color);
//         pos.x() += char_size(height).x();
//     }
// }


} // namespace frog

#endif
