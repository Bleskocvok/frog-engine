#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "frog/core/scene_manager.hpp"
#include "frog/core/game_object2d.hpp"
#include "frog/core/2d/camera.hpp"
#include "frog/core/scene_manager.hpp"
#include "frog/graphics/assets.hpp"
#include "frog/graphics/color.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/font/atlas.hpp"
#include "frog/font/truetype.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/lib2d/window.hpp"
#include "frog/gx2d/crop.hpp"
#include "frog/gx2d/sprite.hpp"

namespace frog::r2d
{

class Renderer
{
    lib2d::gx::window* window;
    frog::gx::assets<lib2d::gx::texture>* textures;
    gx::assets<font::base>* fonts;
    frog::camera2d camera;

    std::pair<geo::vec2, geo::vec2> scale_shift(geo::rect& cam) const;
    std::pair<geo::vec2, geo::vec2> ui_scale_shift() const;

    // Friends and family.
    friend font::atlas;
    friend font::truetype;

    struct RenderCtx
    {
        geo::vec2 scale;
        geo::vec2 shift;

        geo::vec2 prev_scale;
        geo::vec2 prev_shift;

        geo::vec2 scale_mult = { 1 };
        geo::vec2 pos_mult = { 1 };

        bool move_pre_scale = true;
        double between = 0;
    };

    void draw(const RenderCtx& ctx, const lib2d::gx::texture& tex, geo::rect dest,
              geo::rect uv, gx::rgba_t color, gx2d::Crop crop = {});

    void draw(const RenderCtx& ctx, const gx2d::Sprite& sprite);

    void draw_recursive(const RenderCtx& ctx, const gx2d::Sprite& sprite);

public:
    Renderer(lib2d::gx::window& window_,
            frog::gx::assets<lib2d::gx::texture>& textures_,
            gx::assets<font::base>& fonts_,
            frog::camera2d camera_)
        : window(&window_)
        , textures(&textures_)
        , fonts(&fonts_)
        , camera(camera_)
    { }

    void draw_sprite(const lib2d::gx::texture& tex, geo::rect dest,
                           geo::rect uv, gx::rgba_t color, gx2d::Crop crop = {});

    void draw_objects(const frog::scene_manager<frog::game_object2d>& scenes, double between);

    void draw_ui_sprite(const lib2d::gx::texture& tex, geo::rect dest,
                           geo::rect uv, gx::rgba_t color);

    void draw_ui(const frog::scene_manager<frog::game_object2d>& scenes, double between);

    void draw_text(const gx::text& label, geo::vec2 pos,
                   float container_height, frog::gx2d::Crop crop = {});
};

} // namespace frog

#endif
