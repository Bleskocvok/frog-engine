#ifndef NOT_FROG_BUILD_2D

#include "engine2d.hpp"

#include "frog/gx2d/renderer2d.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/gx2d/crop.hpp"
#include "frog/graphics/color.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/font/atlas.hpp"
#include "frog/font/truetype.hpp"
#include "frog/geometry/general.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/utils/exception.hpp"
#include "frog/utils/ptr.hpp"

#include "frog/lib2d/structs.hpp"
#include "frog/lib2d/window.hpp"
#include "frog/core/settings.hpp"

#include <utility>      // move
#include <vector>
#include <tuple>        // tie
#include <algorithm>    // max

using namespace frog::geo;
using namespace frog;

namespace {

frog::lib2d::gx::Mode window_screen_mode(const settings& s)
{
    switch (s.mode) {
        case ScreenMode::Borderless: return frog::lib2d::gx::Mode::Borderless;
        case ScreenMode::Fullscreen: return frog::lib2d::gx::Mode::Fullscreen;
        case ScreenMode::Windowed:   return frog::lib2d::gx::Mode::Windowed;
        default:
            throw error("unsupported screen mode");
    }
}

frog::lib2d::gx::Vsync vsync(const settings& s)
{
    if (s.vsync)
        return frog::lib2d::gx::Vsync::On;
    return frog::lib2d::gx::Vsync::Off;
}

void apply_crop(const gx2d::sprite& model, geo::rect& rect, geo::rect& tex)
{
    if (not model.crop)
        return;

    gx2d::crop_tex(*model.crop, rect, tex);
    gx2d::crop_rect(*model.crop, rect);
}

} // namespace

namespace frog {


engine2d::engine2d(settings set, ptr<state> _global)
    : engine_base( mk_ptr<lib2d::gx::window>(set.width, set.height,
                                             set.window_name.c_str(),
                                             vsync(set),
                                             window_screen_mode(set)),
                   nullptr,
                   std::move(_global) )
    , win_raw(static_cast<lib2d::gx::window*>(window.get()))
{
    renderer = mk_ptr<gx::renderer2d>(static_cast<lib2d::gx::window*>(window.get()));

    if (set.window_icon)
    {
        std::string filename = global->asset_path() + "/" + *set.window_icon;
        lib2d::detail::surface img = lib2d::detail::load_img(filename);
        win_raw->set_icon( img );
    }
}


void engine2d::init()
{
    scenes->init(*this);

    auto prefix = global->asset_path() + "/";
    // TODO: Load font settings from provided .ini file.
    fonts.add("default", mk_ptr<font::atlas>(*this, prefix + "font.png", "oogabooga.ini"));
}

bool engine2d::add_atlas_font(const std::string& tag, std::string path,
                              const std::string& config)
{
    bool has = fonts.contains(tag);

    prepend_path_prefix(path);
    fonts.add(tag, mk_ptr<font::atlas>(*this, path, config));
    return has;
}

bool engine2d::add_truetype_font(const std::string& tag, std::string path,
                                 int size, bool outline)
{
    bool has = fonts.contains(tag);

    prepend_path_prefix(path);
    fonts.add(tag, mk_ptr<font::truetype>(path, size, outline));

    return has;
}

void engine2d::prepend_path_prefix(std::string& path)
{
    if (path.empty())
        return;

    if (path.front() != '/')
        path = global->asset_path() + "/" + path;
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

void engine2d::reset_controls()
{
    input->reset();
}

void engine2d::stable_update()
{
    camera().prev = camera();
    engine_base::stable_update();
}

void engine2d::frame_update()
{
    engine_base::frame_update();
}


geo::vec2 engine2d::camera_coords(int mouse_x, int mouse_y)
{
    geo::vec2 scale = { camera().size.x() / win_raw->w(),
                        camera().size.y() / win_raw->h() };
    geo::vec2 shift = camera().top_left();
    auto mouse = geo::vec2{ float(mouse_x), float(mouse_y) };
    return shift + mouse * scale;
}


geo::vec2 engine2d::camera_coords_ui(int mouse_x, int mouse_y)
{
    const auto& cam = camera();
    geo::vec2 scale = { cam.size.x() / win_raw->w(),
                        cam.size.y() / win_raw->h() };
    geo::rect centered = cam;
    centered.pos = { 0 };
    auto shift = centered.top_left();
    return shift + scale * geo::vec2{ float(mouse_x), float(mouse_y) };
}


std::pair<geo::vec2, geo::vec2> engine2d::scale_shift(geo::rect& cam) const
{
    geo::vec2 scale = { win_raw->w() / cam.size.x(),
                        win_raw->h() / cam.size.y() };
    geo::vec2 shift = { 0 };
    shift -= cam.top_left();
    return { scale, shift };
}

std::pair<geo::vec2, geo::vec2> engine2d::ui_scale_shift() const
{
    geo::vec2 scale = { win_raw->w() / camera().size.x(),
                        win_raw->h() / camera().size.y() };
    geo::vec2 shift = { 0 };
    shift.x() += win_raw->w() * 0.5;
    shift.y() += win_raw->h() * 0.5;
    return { scale, shift };
}


// TODO: unused parameter, use it for extrapolation of movement
void engine2d::draw_objects(double between)
{
    // TODO: Use std::set!
    // TODO: solve in a more appropriate OOP way
    //       also, perhaps create some sort of reference counting with layers
    //       in render queue (i.e. add reference for a new game object, remove for deleted object)
    // TODO: make this more memory efficient for unhinged layer values
    //       (i.e. don't crash the game for unsigned(-1))
    std::vector<std::vector<const gx2d::sprite*>> render_queue;

    geo::vec2 scale;
    geo::vec2 shift;
    std::tie(scale, shift) = scale_shift(camera());

    geo::vec2 prev_scale;
    geo::vec2 prev_shift;
    std::tie(prev_scale, prev_shift) = scale_shift(camera().prev);

    scale = frog::geo::lerp(prev_scale, scale, float(between));
    shift = frog::geo::lerp(prev_shift, shift, float(between));

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
        for (const gx2d::sprite* model : layer)
        {
            auto rect = model->rect;
            float angle = model->angle;

            if (model->interpolation == gx2d::Interpolation::INTERPOLATE)
            {
                rect.pos = frog::geo::lerp(model->prev.pos, model->rect.pos, float(between));
                angle = lerp_deg(model->prev.angle, model->angle, between);
            }
            else if (model->interpolation == gx2d::Interpolation::EXTRAPOLATE)
            {
                rect.pos = frog::geo::lerp(model->prev.pos, model->rect.pos, float(1 + between));
                angle = lerp_deg(model->prev.angle, model->angle, 1 + between);
            }

            // TODO: Apply crop here too.

            rect.pos += shift;
            rect.pos.x() *= scale.x();
            rect.pos.y() *= scale.y();

            rect.size.x() *= scale.x();
            rect.size.y() *= scale.y();
            auto top_left = rect.top_left();

            const auto& it = textures.find(model->image_tag);
            if (not it)
                throw std::runtime_error("invalid texture '" + model->image_tag + "'");
            const auto& tex = *it;

            auto uv_size = geo::vec2{ float(tex.w()), float(tex.h()) } * model->tex.size;
            // auto uv = ( model->tex.pos - model->tex.size * 0.5f )
            //         * geo::vec2{ float(tex.w()), float(tex.h()) };
            auto uv = ( model->tex.pos ) * geo::vec2{ float(tex.w()), float(tex.h()) };
            gx::rgba_t color = model->color;
            win_raw->draw_colored_rotated(tex, uv.x(), uv.y(), uv_size.x(), uv_size.y(),
                                          top_left.x(), top_left.y(),
                                          rect.size.x(), rect.size.y(),
                                          color.r(), color.g(), color.b(), color.a(),
                                          rect.size.x() / 2, rect.size.y() / 2,
                                          angle);
        }
}


void engine2d::draw_text(const gx::text& label, geo::vec2 pos,
                        float container_height, frog::gx2d::Crop crop)
{
    auto& font = fonts.at(label.font);
    font.draw(*this, label, pos, container_height, crop);
}


void engine2d::draw_sprite(const lib2d::gx::texture& tex, geo::rect dest,
                           geo::rect uv, gx::rgba_t color, gx2d::Crop crop)
{
    geo::vec2 scale;
    geo::vec2 shift;
    std::tie(scale, shift) = ui_scale_shift();

    gx2d::crop_tex(crop, dest, uv);
    gx2d::crop_rect(crop, dest);

    dest.pos.x() *= scale.x();
    dest.pos.y() *= scale.y();
    dest.pos += shift;

    dest.size.x() *= scale.x();
    dest.size.y() *= scale.y();
    auto top_left = dest.top_left();

    // TODO: This is a little unhinged. I should probably use the same
    // coordinate system for textures throughout the whole code.
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


void engine2d::draw_ui_sprite(const lib2d::gx::texture& tex, geo::rect dest,
                           geo::rect uv, gx::rgba_t color)
{
    geo::vec2 scale;
    geo::vec2 shift;
    std::tie(scale, shift) = ui_scale_shift();

    dest.pos.x() *= scale.x();
    dest.pos.y() *= scale.y();
    dest.pos += shift;

    dest.size.x() *= scale.x();
    dest.size.y() *= scale.y();
    auto top_left = dest.top_left();

    uv.pos *= geo::vec2(tex.w(), tex.h());
    uv.size *= geo::vec2(tex.w(), tex.h());
    // uv.pos -= uv.size * 0.5;

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
    scenes->for_each_object([&](auto& obj)
    {
        for (const frog::ptr<gx::ui_element>& elem : obj.elements())
        {
            if (not elem->sprite.image_tag.empty())
            {
                auto rect = elem->sprite.rect;
                auto tex = elem->sprite.tex;
                apply_crop(elem->sprite, rect, tex);

                draw_ui_sprite(textures.at(elem->sprite.image_tag),
                            rect,
                            tex,
                            elem->sprite.color);
            }

            if (elem->label)
            {
                gx2d::Crop crop;

                if (elem->sprite.crop)
                {
                    float height = elem->label->height * elem->size().y();
                    float dif = 0.5 * ( elem->sprite.rect.size.y() - height );
                    crop.top = elem->sprite.crop->top - dif;
                    crop.bot = elem->sprite.crop->bot - dif;
                    crop.top = std::max(0.0f, crop.top);
                    crop.bot = std::max(0.0f, crop.bot);
                }

                draw_text(*elem->label, elem->pos(), elem->size().y(), crop);
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
