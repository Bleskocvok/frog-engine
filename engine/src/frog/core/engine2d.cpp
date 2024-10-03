#ifndef NOT_FROG_BUILD_2D

#include "engine2d.hpp"

#include "frog/graphics/color.hpp"

#include <utility>      // move
#include <vector>
#include <tuple>        // tie
#include "frog/utils/debug.hpp"

using namespace frog::geo;
using namespace frog;


namespace frog {


engine2d::engine2d(settings set, ptr<state> _global)
    : engine_base( mk_ptr<lib2d::gx::window>(set.width, set.height, set.window_name.c_str()),
                   nullptr,
                   std::move(_global) )
    , win_raw(static_cast<lib2d::gx::window*>(window.get()))
{
    renderer = mk_ptr<gx::renderer2d>(static_cast<lib2d::gx::window*>(window.get()));
}


void engine2d::init()
{
    scenes->init(*this);
}


void engine2d::update_controls()
{
    input->update();

    // resize renderer if window has been resized
    auto resized = input->has_resized();
    if (resized)
    {
        const auto& [w, h] = *resized;
        renderer->viewport(w, h);
    }

    if (input->quit())
        global->quit = true;
}


void engine2d::stable_update()
{
    engine_base::stable_update();
}

void engine2d::frame_update()
{
    engine_base::frame_update();
}


geo::vec2 engine2d::camera_coords(int mouse_x, int mouse_y)
{
    geo::vec2 scale = { camera.size.x() / win_raw->w(),
                        camera.size.y() / win_raw->h() };
    geo::vec2 shift = { 0 };
    shift += camera.top_left();
    return { mouse_x * scale.x() + shift.x(), mouse_y * scale.y() + shift.y() };
}



std::pair<geo::vec2, geo::vec2> engine2d::scale_shift() const
{
    geo::vec2 scale = { win_raw->w() / camera.size.x(),
                        win_raw->h() / camera.size.y() };
    geo::vec2 shift = { 0 };
    shift.x() += win_raw->w() * 0.5;
    shift.y() += win_raw->h() * 0.5;
    shift.x() -= camera.pos.x();
    shift.y() -= camera.pos.y();
    return { scale, shift };
}


// TODO: unused parameter, use it for extrapolation of movement
void engine2d::draw_objects(double /* between */)
{
    // TODO: solve in a more appropriate OOP way
    //       also, perhaps create some sort of reference counting with layers
    //       in render queue (i.e. add reference for a new game object, remove for deleted object)
    // TODO: make this more memory efficient for unhinged layer values
    //       (i.e. don't crash the game for unsigned(-1))
    std::vector<std::vector<const gx2d::sprite*>> render_queue;

    geo::vec2 scale;
    geo::vec2 shift;
    std::tie(scale, shift) = scale_shift();

    scenes->for_each_object([&](const auto& obj)
        {
            if (obj.model().image_tag.empty())
                return;

            auto layer = obj.model().layer;

            if (layer >= render_queue.size())
                render_queue.resize(layer + 1);

            render_queue[layer].push_back(&obj.model());
        });

    for (const auto& layer : render_queue)
        for (const auto& model : layer)
        {
            auto rect = model->rect;
            rect.pos.x() *= scale.x();
            rect.pos.y() *= scale.y();
            rect.pos += shift;

            rect.size.x() *= scale.x();
            rect.size.y() *= scale.y();
            auto top_left = rect.top_left();

            // TODO: account for rotation, color etc.
            // win_raw->draw(textures.at(model->image_tag), top_left.x(), top_left.y(),
            //               rect.size.x(), rect.size.y());
            // const auto& tex = textures.at(model->image_tag);
            const auto& it = textures.find(model->image_tag);
            if (!it)
                throw std::runtime_error("invalid texture '" + model->image_tag + "'");
            const auto& tex = *it;

            auto uv_size = geo::vec2{ tex.w(), tex.h() } * model->tex.size;
            auto uv = ( model->tex.pos - model->tex.size / 2 )
                    * geo::vec2{ tex.h(), tex.w() };

            gx::rgba_t color = gx::color_to_255(model->color);
            win_raw->draw_colored_rotated(tex, uv.x(), uv.y(), uv_size.x(), uv_size.y(),
                                          top_left.x(), top_left.y(),
                                          rect.size.x(), rect.size.y(),
                                          color.r(), color.g(), color.b(), color.a(),
                                          rect.size.x() / 2, rect.size.y() / 2,
                                          model->angle);
        }
}


void engine2d::draw_text(const lib2d::gx::texture& tex, std::string_view str,
               geo::vec2 pos, float height, gx::rgba_t color, bool centered)
{
    static const std::string map = "ABCDEFGHIJKLMNOP"
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
                    / 2;
    }
    else
    {
        pos.x() += height / 2;
    }

    for (char c : str)
    {
        size_t idx = std::distance(map.begin(),
                                   std::find(map.begin(), map.end(), c));

        if (idx >= map.size())
        {
            continue;
        }

        geo::vec2 tex_size = { 1.f / grid_size,
                               size_ratio / grid_size };

        geo::vec2 tex_pos = { static_cast<float>(idx % grid_size),
                              static_cast<float>(idx / grid_size) };

        tex_pos *= tex_size;

        draw_sprite(tex, { pos, vec2{ height } }, { tex_pos, tex_size }, color);
        pos.x() += height * spacing * size_ratio;
    }
}


void engine2d::draw_sprite(const lib2d::gx::texture& tex, geo::rect dest,
                           geo::rect uv, gx::rgba_t color)
{
    geo::vec2 scale;
    geo::vec2 shift;
    std::tie(scale, shift) = scale_shift();

    dest.pos.x() *= scale.x();
    dest.pos.y() *= scale.y();
    dest.pos += shift;

    dest.size.x() *= scale.x();
    dest.size.y() *= scale.y();
    auto top_left = dest.top_left();

    uv.pos *= geo::vec2(tex.w(), tex.h());
    uv.size *= geo::vec2(tex.w(), tex.h());

    win_raw->draw_colored_rotated(tex, uv.pos.x(), uv.pos.y(),
                                  uv.size.x(), uv.size.y(),
                                  top_left.x(), top_left.y(),
                                  dest.size.x(), dest.size.y(),
                                  color.r(), color.g(), color.b(), color.a(),
                                  dest.size.x() / 2, dest.size.y() / 2,
                                  0);
}


void engine2d::draw_ui(double)
{
    const auto& tex = textures.at("font");

    scenes->for_each_object([&](auto& obj)
    {
        for (const auto& elem : obj.elements())
        {
            if (!elem->sprite.empty())
            {
                draw_sprite(textures.at(elem->sprite),
                            { elem->pos, elem->size },
                            { elem->tex_pos, elem->tex_size },
                            elem->color);
            }

            if (elem->label)
            {
                // draw_sprite(textures.at("box"),
                //             { elem->pos, elem->size },
                //             { elem->tex_pos, elem->tex_size },
                //             gx::rgba_t{ 200, 0, 0, 255 });
                draw_text(tex, elem->label->str,
                          elem->pos,
                          elem->size.y() * elem->label->height,
                          elem->color,
                          elem->label->centered);
            }
        }
    });
}


bool engine2d::add_texture(const std::string& tag, const std::string& path)
{
    bool has = textures.contains(tag);
    auto full = global->asset_path() + "/" + path;
    textures.add(tag, mk_ptr<lib2d::gx::texture>( win_raw->make_texture(full.c_str()) ));
    return has;
}


bool engine2d::remove_texture(const std::string& tag)
{
    return textures.remove(tag);
}


}  // namespace engine2d

#endif
