#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "frog/graphics/assets.hpp"

#include "frog/geometry/rectangle.hpp"
#include "frog/utils/ptr.hpp"

// bits, please
#include "frog/lib2d/bits.hpp"      // initializer

#include "engine_base.hpp"
#include "game_object2d.hpp"
#include "settings.hpp"
#include "state.hpp"

#include "frog/font/base.hpp"

#include <string>
// NOLINTNEXTLINE
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

    void draw_text(const std::string& font_name, const std::string& str,
                   geo::vec2 pos, float height, gx::rgba_t color, bool centered);

    void draw_sprite(const lib2d::gx::texture& tex, geo::rect dest, geo::rect uv,
                     gx::rgba_t color);

    void draw_ui_sprite(const lib2d::gx::texture& tex, geo::rect dest, geo::rect uv,
                        gx::rgba_t color);

    void prepend_path_prefix(std::string& path);

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

    bool add_atlas_font(const std::string& tag, std::string path,
                        const std::string& config);

    bool add_truetype_font(const std::string& tag, std::string path,
                           int size = 64, bool outline = false);
};


template<typename InitAssets, typename AddObjects>
int run_engine2d(settings set, ptr<state> global, InitAssets init_assets,
                 AddObjects add_objects)
{
    using BaseException = std::exception;

    auto handle_exception = [&]<typename T>(const T& ex, const char* title)
    {
        if constexpr (requires{ ex.stacktrace; })
        {
            auto str = ex.what() + std::string("\n\n") + ex.stacktrace;
            lib2d::os::error_box(title, str);
        }
        else
        {
            auto str = ex.what() + std::string("\n\n");
            lib2d::os::error_box(title, str);
        }
    };

    auto with_catch = [&](const char* title, auto func) -> bool
    {
        try
        {
            func();
        }
        catch (frog::error& ex)
        {
            handle_exception(ex, title);
            return false;
        }
        catch (BaseException& ex)
        {
            handle_exception(ex, title);
            return false;
        }
        return true;
    };

    auto engine = frog::ptr<frog::engine2d>{ nullptr };

    if (not with_catch("FATAL ERROR: engine creation failed",
                    [&](){
                        engine = frog::mk_ptr<frog::engine2d>(std::move(set), std::move(global));
                    })
     || not with_catch("ERROR: engine failed during loading",
                    [&](){
                        init_assets(*engine);
                        add_objects(*engine);
                     })
     || not with_catch("ERROR: engine crashed",
                    [&](){
                        engine->play();
                    })
     )
        return 1;

    return 0;
}


} // namespace frog


#endif
