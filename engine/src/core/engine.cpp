#include "engine.hpp"

#include "os/timer.hpp"

#include "geometry/transform.hpp"
#include "graphics/program.hpp"
#include "graphics/location.hpp"
#include "graphics/shape.hpp"
#include "graphics/mesh.hpp"
#include "graphics/material.hpp"
#include "graphics/model.hpp"

#include <utility>      // std::move
#include <algorithm>    // std::find
#include <iterator>     // std::distance


using namespace frog::geo;
using namespace frog::gl;
using namespace frog;


frog::engine::engine(settings set, ptr<state> _global)
    : window(set.width, set.height, set.window_name.c_str())
    , renderer(set.gl_major, set.gl_minor, set.vsync)
    , global(std::move(_global))
{
    window.make_current_context();

    renderer.viewport(set.width, set.height);
    renderer.clear_color(set.clear_color.r
                        , set.clear_color.g
                        , set.clear_color.b);

    // so that no callbacks are missed
    // (actually not needed unless we use polling functions, e.g. getKeyState)
    // window.set_input_mode(GLFW_STICKY_KEYS, GLFW_TRUE);

    window.set_callbacks(input.get());

    camera.screen_size(set.width, set.height);
}


void frog::engine::play()
{
    os::timer timer;
    // DO NOT MAKE THIS VARIABLE UNSIGNED!
    int accum = 0;
    unsigned delta = global->physics_delta_us;

    init();

    while (!global->quit && !window.should_close())
    {
        frame_update();

        while (accum >= delta)
        {
            update_controls();
            stable_update();
            accum -= delta;
        }

        auto frame = timer.reset_duration<os::us>();
        global->frame_time_us = frame;
        accum += frame;

        render(frame / static_cast<double>(delta));
    }
}


void frog::engine::init()
{
    programs.add("main", program("engine/assets/main.vert",
                                 "engine/assets/main.frag"));

    programs.add("ui", program("engine/assets/ui.vert",
                               "engine/assets/ui.frag"));

    textures.add("", texture("engine/assets/empty.png"));

    textures.add("font", texture("engine/assets/font.png"));

    scenes->init(*this);
}


void frog::engine::update_controls()
{
    input->reset();
    glfwPollEvents();

    // resize renderer if window has been resized
    if (input->has_resized())
    {
        const auto& [w, h] = *(input->has_resized());
        renderer.viewport(w, h);
        camera.screen_size(w, h);
    }
}


void frog::engine::stable_update()
{
    scenes->stable_update(*this);
    scenes->cleanup(*this);
}


void frog::engine::frame_update()
{
    scenes->frame_update(*this);
}


void frog::engine::render(double between)
{
    renderer.clear();

    renderer.prepare3D();

    draw3D(between);

    renderer.prepare2D();

    drawUI(between);

    window.swap_buffers();
}


// unused parameter, TODO use it for extrapolation of movement
void frog::engine::draw3D(double /* between */)
{
    auto& main = programs.at("main");
    main.use();

    main.uniform_mat(camera.projection(), location::ProjectionMatrix);
    main.uniform_mat(camera.view(), location::ViewMatrix);

    main.uniform_vec(sun, location::LightPos);
    main.uniform_vec(camera.eye_position(), location::EyePos);

    scenes->for_each_object([&](auto& obj)
        {
            if (obj.model().mesh_tag.empty())
                return;

            const auto& model = obj.model();

            model.mat.send(main, location::Material);
            textures.at(model.texture_tag).bind(location::TextureBinding);
            main.uniform_mat(model.matrix(), location::ModelMatrix);
            meshes.at(model.mesh_tag).draw();
        });
}


void frog::engine::drawUI(double)
{
    auto& ui = programs.at("ui");
    ui.use();

    scenes->for_each_object([&](auto& obj)
    {
        for (const auto& elem : obj.elements())
        {
            if (!elem->sprite.empty())
            {
                draw_sprite(ui,
                            elem->sprite,
                            { elem->pos.x() / camera.ratio(), elem->pos.y() },
                            elem->size,
                            elem->tex_pos,
                            elem->tex_size,
                            elem->color);
            }

            if (elem->label)
            {
                draw_text(ui,
                          elem->label->str,
                          elem->pos,
                          elem->size.y() * elem->label->height,
                          elem->color,
                          elem->label->centered);
            }
        }
    });
}


void frog::engine::draw_sprite(gl::program& ui,
                               const std::string& texture,
                               geo::vec2 pos,
                               geo::vec2 size,
                               geo::vec2 tex_pos,
                               geo::vec2 tex_size,
                               geo::vec4 color)
{
    textures.at(texture).bind(location::TextureBinding);

    size.x() /= camera.ratio();

    ui.uniform_vec(pos, location::ui::Pos);
    ui.uniform_vec(size, location::ui::Size);
    ui.uniform_vec(tex_pos, location::ui::TexPos);
    ui.uniform_vec(tex_size, location::ui::TexSize);
    ui.uniform_vec(color, location::ui::TexColor);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void frog::engine::draw_text(gl::program& ui,
                             std::string_view str,
                             geo::vec2 pos,
                             float height,
                             geo::vec4 color,
                             bool centered)
{
    static const std::string map =
            "ABCDEFGHIJKLMNOP"
            "QRSTUVWXYZ012345"
            "6789 .,!?-:/_'#*"
            "\"+abcdefghijklmn"
            "opqrstuvwxyz=()";
    static const int grid_size = 16;
    static const float size_ratio = 1;
    static const float spacing = 0.75;

    if (centered)
    {
        pos.x() -= str.length()
                    * height
                    * spacing
                    * size_ratio
                    / camera.ratio()
                    / 2;
    }

    for (char ch : str)
    {
        size_t idx = std::distance(map.begin(),
                                   std::find(map.begin(), map.end(), ch));

        if (idx >= map.size())
        {
            continue;
        }

        geo::vec2 tex_size = { 1.f / grid_size,
                               size_ratio / grid_size };

        geo::vec2 tex_pos = { static_cast<float>(idx % grid_size),
                              static_cast<float>(idx / grid_size) };

        tex_pos.x() *= tex_size.x();
        tex_pos.y() *= tex_size.y();

        draw_sprite(ui, "font", pos, vec2{ height }, tex_pos, tex_size, color);
        pos.x() += height * spacing * size_ratio / camera.ratio();
    }
}
