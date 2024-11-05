#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "frog/gx3d/include.hpp"

#include "frog/geometry/camera.hpp"
#include "frog/graphics/renderer3d.hpp"
#include "frog/graphics/assets.hpp"
#include "frog/gx3d/texture.hpp"
#include "frog/gx3d/mesh.hpp"
#include "frog/os/window.hpp"
#include "frog/os/input.hpp"
#include "frog/os/timer.hpp"
#include "frog/utils/ptr.hpp"

#include "engine_base.hpp"
#include "scene_manager.hpp"
#include "game_object.hpp"
#include "settings.hpp"
#include "state.hpp"

#include <string_view>
#include <utility>      // move


namespace frog {


class engine : public engine_base<engine, game_object, os::timer>
{
    os::GLFW glfw{};

    void draw_objects(double between) override;
    void draw_ui(double between) override;

    void update_controls() override;

    void stable_update() override;
    void frame_update() override;

    void draw_sprite(
        gl::program& ui,
        const std::string& texture,
        geo::vec2 pos,
        geo::vec2 size,
        geo::vec2 tex_pos = { 0, 0 },
        geo::vec2 tex_size = { 1, 1 },
        geo::vec4 color = { 1, 1, 1, 1 });

    void draw_text(
        gl::program& ui,
        std::string_view,
        geo::vec2 pos,
        float height,
        geo::vec4 color = { 1, 1, 1, 1 },
        bool centered = false);

    void init() override;

public:
    os::window* win_raw = nullptr;
    ptr<os::input> input = mk_ptr<os::input>();

    frog::geo::camera camera{ 100, 0.1, 100 };
    geo::vec4 sun = { 7, 7, 7, 0 };

    gx::assets<gl::mesh> meshes;
    gx::assets<gl::program> programs;
    gx::assets<gl::texture> textures;

    engine(settings set, ptr<state> global);

    bool add_texture(const std::string& tag, const std::string& path);
    bool remove_texture(const std::string& name);
};


} // namespace frog


#endif
