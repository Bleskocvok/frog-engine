#pragma once

#include "graphics/renderer.hpp"
#include "graphics/texture.hpp"
#include "graphics/assets.hpp"
#include "graphics/mesh.hpp"

#include "os/window.hpp"
#include "os/input.hpp"

#include "utils/ptr.hpp"

#include "scene_manager.hpp"
#include "settings.hpp"
#include "camera.hpp"
#include "state.hpp"

#include <string_view>


namespace frog
{


class engine
{
    os::GLFW glfw{};
    os::window window;
    gl::renderer renderer;

    void render(double between);

    void draw3D(double between);
    void drawUI(double between);

    void update_controls();

    void init();

    void stable_update();

    void frame_update();

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

public:
    ptr<scene_manager> scenes = mk_ptr<scene_manager>();
    ptr<os::input> input = mk_ptr<os::input>();
    ptr<state> global;

    frog::camera camera{ 100, 0.1, 100 };
    geo::vec4 sun = { 7, 7, 7, 0 };

    gl::assets<gl::mesh> meshes;
    gl::assets<gl::program> programs;
    gl::assets<gl::texture> textures;

    engine(settings set, ptr<state> global);

    void play();

};


} // namespace frog
