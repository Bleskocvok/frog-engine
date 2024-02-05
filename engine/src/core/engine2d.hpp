#ifndef NOT_FROG_BUILD_2D

#pragma once

#include "graphics/renderer2d.hpp"
#include "graphics/assets.hpp"

// bits, please
#include "lib2d/bits.hpp"

#include "utils/ptr.hpp"

#include "engine_base.hpp"
#include "game_object2d.hpp"
#include "scene_manager.hpp"
#include "settings.hpp"
#include "state.hpp"
#include "geometry/rectangle.hpp"

#include <string_view>
#include <utility>      // move


namespace frog {


class engine2d : public engine_base<engine2d, game_object2d, lib2d::os::timer>
{
    void draw3D(double between) override;
    void drawUI(double between) override;

    void update_controls() override;

    void stable_update() override;
    void frame_update() override;

    void init() override;

public:
    lib2d::gx::window* win_raw = nullptr;
    ptr<lib2d::gx::events> input = mk_ptr<lib2d::gx::events>();

    gx::assets<lib2d::gx::texture> textures;

    geo::rect camera = { geo::vec2{ 0 }, geo::vec2{ 1, 1 } };

    engine2d(settings set, ptr<state> global);

    bool add_texture(const std::string& tag, const std::string& path);
    bool remove_texture(const std::string& name);
};


} // namespace frog


#endif
