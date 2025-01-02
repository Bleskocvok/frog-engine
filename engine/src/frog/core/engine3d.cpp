#ifndef NOT_FROG_BUILD_3D

#include "engine3d.hpp"

#include "frog/graphics/renderer3d.hpp"
#include "frog/gx3d/program.hpp"
#include "frog/gx3d/location.hpp"

#include <utility>      // std::move
#include <algorithm>    // std::find
#include <iterator>     // std::distance


using namespace frog::geo;
using namespace frog::gl;
using namespace frog;


frog::engine::engine(settings set, ptr<state> _global)
    : engine_base( nullptr,
                   nullptr,
                   std::move(_global) )
{
    // TODO: Debug on WSL why this segfaults
    // LOG("pre-win");
    window = mk_ptr<os::window>(set.width, set.height, set.window_name.c_str());
    // LOG("post-win");
    win_raw = static_cast<os::window*>(window.get());
    // LOG("post-win-raw");

    renderer = mk_ptr<gl::renderer3d>(set.gl_major, set.gl_minor, set.vsync);

    // LOG("viewport");

    renderer->viewport(set.width, set.height);
    renderer->clear_color(set.clear_color.r
                        , set.clear_color.g
                        , set.clear_color.b);

    // LOG("post-view");

    // so that no callbacks are missed
    // (actually not needed unless we use polling functions, e.g. getKeyState)
    // window.set_input_mode(GLFW_STICKY_KEYS, GLFW_TRUE);

    win_raw->set_callbacks(input.get());

    // LOG("win_raw");

    camera.screen_size(set.width, set.height);

    // LOG("engine ok");
}


void frog::engine::init()
{
    // TODO: Get prefix path from somewhere (config from constructor probably?).

    programs.add("main", program("engine/assets/main.vert",
                                 "engine/assets/main.frag"));

    programs.add("ui", program("engine/assets/ui.vert",
                               "engine/assets/ui.frag"));

    textures.add("", texture("engine/assets/empty.png"));

    textures.add("font", texture("engine/assets/font.png"));

    scenes->init(*this);
}


void frog::engine::stable_update()
{
    engine_base::stable_update();
}


void frog::engine::frame_update()
{
    engine_base::frame_update();
}


void frog::engine::update_controls()
{
    // This was moved to the new method ‹reset_controls›.
    // input->reset();
    glfw.poll_events();

    // resize renderer if window has been resized
    auto resized = input->has_resized();
    if (resized)
    {
        const auto& [w, h] = *resized;
        renderer->viewport(w, h);
        camera.screen_size(w, h);
    }
}

void frog::engine::reset_controls()
{
    input->reset();
}


// unused parameter, TODO use it for extrapolation of movement
void frog::engine::draw_objects(double /* between */)
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


void frog::engine::draw_ui(double)
{
    auto& ui = programs.at("ui");
    ui.use();

    scenes->for_each_object([&](frog::game_object& obj)
    {
        for (const auto& elem : obj.elements())
        {
            if (!elem->sprite.image_tag.empty())
            {
                draw_sprite(ui,
                            elem->sprite.image_tag,
                            { elem->pos().x() / camera.ratio(), elem->pos().y() },
                            elem->size(),
                            elem->tex_pos(),
                            elem->tex_size(),
                            gx::rgb_to_vec(elem->color()));
            }

            if (elem->label)
            {
                draw_text(ui,
                          elem->label->str,
                          elem->pos(),
                          elem->size().y() * elem->label->height,
                          gx::rgb_to_vec(elem->color()),
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


bool frog::engine::add_texture(const std::string& tag, const std::string& path)
{
    bool has = textures.contains(tag);
    auto full = global->asset_path() + "/" + path;
    textures.add(tag, gl::texture{ std::move(full) });
    return has;
}


bool frog::engine::remove_texture(const std::string& tag)
{
    return textures.remove(tag);
}


#endif
