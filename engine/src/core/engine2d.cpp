#ifndef NOT_FROG_BUILD_2D

#include "engine2d.hpp"

#include "graphics/renderer2d.hpp"

#include <utility>      // std::move
#include <algorithm>    // std::find
#include <iterator>     // std::distance

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


// unused parameter, TODO use it for extrapolation of movement
void engine2d::draw_objects(double /* between */)
{
    geo::vec2 scale = { win_raw->w() / camera.size.x(),
                        win_raw->h() / camera.size.y() };
    geo::vec2 shift = { 0 };
    shift.x() += win_raw->w() * 0.5;
    shift.y() += win_raw->h() * 0.5;
    shift.x() -= camera.pos.x();
    shift.y() -= camera.pos.y();

    scenes->for_each_object([&](auto& obj)
        {
            if (obj.model().image_tag.empty())
                return;

            const auto& model = obj.model();

            auto rect = model.rect;
            rect.pos.x() *= scale.x();
            rect.pos.y() *= scale.y();
            rect.pos += shift;

            rect.size.x() *= scale.x();
            rect.size.y() *= scale.y();
            auto top_left = rect.top_left();

            win_raw->draw(textures.at(model.image_tag), top_left.x(), top_left.y(),
                          rect.size.x(), rect.size.y());
        });
}


void engine2d::draw_ui(double)
{
    // TODO
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
