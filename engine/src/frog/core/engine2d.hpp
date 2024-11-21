#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "frog/gx2d/renderer2d.hpp"
#include "frog/graphics/assets.hpp"

#include "frog/geometry/rectangle.hpp"
#include "frog/utils/ptr.hpp"

// bits, please
#include "frog/lib2d/bits.hpp"

#include "engine_base.hpp"
#include "game_object2d.hpp"
#include "scene_manager.hpp"
#include "settings.hpp"
#include "state.hpp"

#include "frog/font/base.hpp"

#include <string>
#include <stdexcept>    // exception
#include <utility>      // move, pair


namespace frog::font { class atlas; class truetype; } // namespace font


namespace frog {


class engine2d : public engine_base<engine2d, game_object2d, lib2d::os::timer>
{
    void draw_objects(double between) override;
    void draw_ui(double between) override;

    void update_controls() override;
    void reset_controls() override;

    void stable_update() override;
    void frame_update() override;

    void init() override;

    std::pair<geo::vec2, geo::vec2> scale_shift() const;

    void draw_text(const std::string& str, geo::vec2 pos, float height,
                   gx::rgba_t color, bool centered);

    void draw_sprite(const lib2d::gx::texture& tex, geo::rect dest, geo::rect uv,
                     gx::rgba_t color);

    void draw_ui_sprite(const lib2d::gx::texture& tex, geo::rect dest, geo::rect uv,
                        gx::rgba_t color);

    // Friends and family.
    friend font::atlas;
    friend font::truetype;

public:
    lib2d::initializer initializer_{ lib2d::initializer::Video };

    lib2d::gx::window* win_raw = nullptr;
    ptr<lib2d::gx::events> input = mk_ptr<lib2d::gx::events>();

    gx::assets<lib2d::gx::texture> textures;
    gx::assets<font::base> fonts;

    geo::rect camera = { geo::vec2{ 0 }, geo::vec2{ 1, 1 } };

    engine2d(settings set, ptr<state> global);

    geo::vec2 mouse_pos_in_camera()
    {
        return camera_coords(input->mouse().x, input->mouse().y);
    }

    geo::vec2 camera_coords(const lib2d::gx::events::mouse_t& m)
    {
        return camera_coords(m.x, m.y);
    }

    geo::vec2 camera_coords(int mouse_x, int mouse_y);

    bool add_texture(const std::string& tag, const std::string& path);
    bool remove_texture(const std::string& name);
};


template<typename InitAssets, typename AddObjects>
int run_engine2d(settings set, ptr<state> global, InitAssets init_assets,
                 AddObjects add_objects)
{
    using BaseException = std::exception;

    auto engine = frog::ptr<frog::engine2d>{ nullptr };

    try
    {
        engine = frog::mk_ptr<frog::engine2d>(std::move(set), std::move(global));
    }
    catch (BaseException& ex)
    {
        lib2d::os::error_box("FATAL ERROR: engine creation failed: ", ex.what());
        return 1;
    }

    try
    {
        init_assets(*engine);
        add_objects(*engine);
    }
    catch (BaseException& ex)
    {
        lib2d::os::error_box("ERROR: engine failed during loading", ex.what());
        return 1;
    }

    try
    {
        engine->play();
    }
    catch (BaseException& ex)
    {
        lib2d::os::error_box("ERROR: engine crashed", ex.what());
        return 1;
    }
    return 0;
}


} // namespace frog


#endif
