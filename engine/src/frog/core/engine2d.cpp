#include "frog/core/2d/renderer.hpp"
#ifndef NOT_FROG_BUILD_2D

#include "engine2d.hpp"

#include "frog/gx2d/renderer2d.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/gx2d/crop.hpp"
#include "frog/graphics/color.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/font/atlas.hpp"
#include "frog/font/truetype.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/utils/exception.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/utils/assert.hpp"

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
    switch (s.window.mode)
    {
        case ScreenMode::Borderless: return frog::lib2d::gx::Mode::Borderless;
        case ScreenMode::Fullscreen: return frog::lib2d::gx::Mode::Fullscreen;
        case ScreenMode::Windowed:   return frog::lib2d::gx::Mode::Windowed;
        default:
            throw error("unsupported screen mode");
    }
}

frog::lib2d::gx::Vsync vsync(const settings& s)
{
    if (s.window.vsync)
        return frog::lib2d::gx::Vsync::On;
    return frog::lib2d::gx::Vsync::Off;
}

lib2d::gx::window_settings get_window_settings(const settings& s)
{
    return lib2d::gx::window_settings
    {
        .width = s.window.width,
        .height = s.window.height,
        .title = s.window.title,
        .vsync = vsync(s),
        .mode = window_screen_mode(s),
        .maximized = s.window.maximized,
        .pos_x = s.window.pos_x,
        .pos_y = s.window.pos_y,
    };
}

} // namespace

namespace frog {


engine2d::engine2d(settings set, ptr<state> _global)
    // : engine_base( mk_ptr<lib2d::gx::window>(set.width, set.height,
    //                                          set.window_name.c_str(),
    //                                          vsync(set),
    //                                          window_screen_mode(set)),
    : engine_base( mk_ptr<lib2d::gx::window>(get_window_settings(set)),
                   nullptr,
                   std::move(_global) )
    , win_raw(static_cast<lib2d::gx::window*>(window.get()))
{
    renderer = mk_ptr<gx::renderer2d>(static_cast<lib2d::gx::window*>(window.get()));

    if (set.window.icon)
    {
        std::string filename = global->asset_path() + "/" + *set.window.icon;
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

void engine2d::draw_objects(double between)
{
    auto renderer = r2d::Renderer(*win_raw, textures, fonts, camera());
    renderer.draw_objects(*scenes, between);
}

// void engine2d::draw_text(const gx::text& label, geo::vec2 pos,
//                         float container_height, frog::gx2d::Crop crop)
// {
//     throw 0;
//     // auto& font = fonts.at(label.font);
//     // font.draw(*this, label, pos, container_height, crop);
// }

// void engine2d::draw_sprite(const lib2d::gx::texture& tex, geo::rect dest,
//                            geo::rect uv, gx::rgba_t color, gx2d::Crop crop)
// {
//     throw 2;
// }

// void engine2d::draw_ui_sprite(const lib2d::gx::texture& tex, geo::rect dest,
//                            geo::rect uv, gx::rgba_t color)
// {
//     throw 2;
// }

void engine2d::draw_ui(double between)
{
    auto renderer = r2d::Renderer(*win_raw, textures, fonts, camera());
    renderer.draw_ui(*scenes, between);
}

bool engine2d::add_texture(const std::string& tag, const std::string& path)
{
    bool has = textures.contains(tag);
    auto full = global->asset_path() + "/" + path;
    textures.add(tag, mk_ptr<lib2d::gx::texture>( win_raw->make_texture(full.c_str()) ));
    // TODO: Omg, I fixed this, I sure hope it didn't break anything else.
    return not has;
}

bool engine2d::remove_texture(const std::string& tag)
{
    return textures.remove(tag);
}

bool engine2d::add_cursor(const std::string& tag, const std::string& path)
{
    bool has = textures.contains(tag);
    auto full = global->asset_path() + "/" + path;

    auto surface = lib2d::detail::load_img( full );
    frog_assert(surface);
    cursors.add(tag, mk_ptr<lib2d::detail::cursor>( SDL_CreateColorCursor( surface.get(), 0, 0 ) ));
    return has;
}

bool engine2d::remove_cursor(const std::string& tag)
{
    return cursors.remove(tag);
}

void engine2d::set_cursor(const std::string& tag)
{
    auto& cursor = cursors.at(tag);
    // TODO: Doesn't belong here.
    SDL_SetCursor(cursor.get());
}

void engine2d::reset_cursor()
{
    auto* def = SDL_CreateSystemCursor( SDL_SystemCursor::SDL_SYSTEM_CURSOR_ARROW );
    auto cursor = lib2d::detail::cursor( def );
    // TODO: Doesn't belong here.
    SDL_SetCursor(cursor.get());
}

}  // namespace engine2d

#endif
