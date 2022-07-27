#include "render_context.hpp"


void frog::os_context::poll_events()
{
    glfwPollEvents();
}



void frog::os_context::draw_sprite(gl::program& ui,
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


void frog::os_context::draw_text(gl::program& ui,
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



// unused parameter, TODO use it for extrapolation of movement
void frog::os_context::draw3D(double /* between */)
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


void frog::os_context::drawUI(double)
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

