#ifndef NOT_FROG_BUILD_2D

#pragma once

#include "graphics/renderer2d.hpp"
#include "graphics/assets.hpp"

// bits, please
#include "lib2d/bits.hpp"

// #include "wrap/window.hpp"
// #include "wrap/input.hpp"
// #include "wrap/timer.hpp"

#include "utils/ptr.hpp"

#include "engine_base.hpp"
#include "game_object2d.hpp"
#include "scene_manager.hpp"
#include "settings.hpp"
#include "state.hpp"

#include <string_view>
#include <utility>      // move


namespace frog {


class engine2d : public engine_base<game_object2d, lib2d::os::timer>
{
    void draw3D(double between) override;
    void drawUI(double between) override;

    void update_controls() override;

    void stable_update() override;
    void frame_update() override;

    // void draw_sprite(
    //     gl::program& ui,
    //     const std::string& texture,
    //     geo::vec2 pos,
    //     geo::vec2 size,
    //     geo::vec2 tex_pos = { 0, 0 },
    //     geo::vec2 tex_size = { 1, 1 },
    //     geo::vec4 color = { 1, 1, 1, 1 });

    // void draw_text(
    //     gl::program& ui,
    //     std::string_view,
    //     geo::vec2 pos,
    //     float height,
    //     geo::vec4 color = { 1, 1, 1, 1 },
    //     bool centered = false);

    void init() override;

public:
    lib2d::gx::window* win_raw = nullptr;
    ptr<lib2d::gx::events> input = mk_ptr<lib2d::gx::events>();

    engine2d(settings set, ptr<state> global);
};


} // namespace frog


#endif
