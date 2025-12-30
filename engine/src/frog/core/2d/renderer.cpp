#ifndef NOT_FROG_BUILD_2D

#include "renderer.hpp"

#include "frog/gx2d/crop.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/utils/assert.hpp"
#include "frog/geometry/rectangle.hpp"
#include "frog/graphics/ui_element.hpp"

#include <map>

using namespace frog::r2d;
using namespace frog;

namespace
{


} // namespace

std::pair<geo::vec2, geo::vec2> Renderer::scale_shift(geo::rect& cam) const
{
    geo::vec2 scale = { window->w() / cam.size.x(),
                        window->h() / cam.size.y() };
    geo::vec2 shift = { 0 };
    shift -= cam.top_left();
    return { scale, shift };
}

std::pair<geo::vec2, geo::vec2> Renderer::ui_scale_shift() const
{
    geo::vec2 scale = { window->w() / camera.size.x(),
                        window->h() / camera.size.y() };
    geo::vec2 shift = { 0 };
    shift.x() += window->w() * 0.5;
    shift.y() += window->h() * 0.5;
    return { scale, shift };
}


void Renderer::draw(const RenderCtx& ctx, const lib2d::gx::texture& tex, geo::rect dest,
                    geo::rect uv, gx::rgba_t color, gx2d::Crop crop)
{
    gx2d::crop_tex(crop, dest, uv);
    gx2d::crop_rect(crop, dest);

    if (ctx.move_pre_scale)
        dest.pos += ctx.shift;

    dest.pos.x() *= ctx.scale.x();
    dest.pos.y() *= ctx.scale.y();

    if (not ctx.move_pre_scale)
        dest.pos += ctx.shift;

    dest.size.x() *= ctx.scale.x();
    dest.size.y() *= ctx.scale.y();
    auto top_left = dest.top_left();

    uv.pos *= geo::vec2(tex.w(), tex.h());
    uv.size *= geo::vec2(tex.w(), tex.h());

    window->draw_colored_rotated(tex, uv.pos.x(), uv.pos.y(),
                                 uv.size.x(), uv.size.y(),
                                 top_left.x(), top_left.y(),
                                 dest.size.x(), dest.size.y(),
                                 color.r(), color.g(), color.b(), color.a(),
                                 dest.size.x() / 2, dest.size.y() / 2,
                                 0);
}

void Renderer::draw(const RenderCtx& ctx, const gx2d::Sprite& model)
{
    if (model.image_tag.empty())
        return;

    auto rect = model.rect;
    float angle = model.angle;

    perform_interpolation(model, ctx.between, rect, angle);

    // TODO: Apply crop here too.

    rect.pos *= ctx.pos_mult;
    rect.size *= ctx.scale_mult;

    rect.pos += ctx.shift;
    rect.pos.x() *= ctx.scale.x();
    rect.pos.y() *= ctx.scale.y();

    rect.size.x() *= ctx.scale.x();
    rect.size.y() *= ctx.scale.y();
    auto top_left = rect.top_left();

    const auto& it = textures->find(model.image_tag);
    if (not it)
        throw std::runtime_error("invalid texture '" + model.image_tag + "'");
    const auto& tex = *it;

    auto uv_size = model.tex.size * geo::vec2{ float(tex.w()), float(tex.h()) };
    auto uv      = model.tex.pos  * geo::vec2{ float(tex.w()), float(tex.h()) };
    gx::rgba_t color = model.color;
    window->draw_colored_rotated(tex, uv.x(), uv.y(), uv_size.x(), uv_size.y(),
                                  top_left.x(), top_left.y(),
                                  rect.size.x(), rect.size.y(),
                                  color.r(), color.g(), color.b(), color.a(),
                                  rect.size.x() / 2, rect.size.y() / 2,
                                  angle);
}

void Renderer::draw_sprite(const lib2d::gx::texture& tex, geo::rect dest,
                       geo::rect uv, gx::rgba_t color, gx2d::Crop crop)
{
    RenderCtx ctx;
    std::tie(ctx.scale, ctx.shift) = ui_scale_shift();

    ctx.move_pre_scale = false;

    draw(ctx, tex, dest, uv, color, crop);
}

void Renderer::draw_ui_sprite(const lib2d::gx::texture& tex, geo::rect dest,
                              geo::rect uv, gx::rgba_t color)
{
    draw_sprite(tex, dest, uv, color);
}

void Renderer::draw_text(const gx::text& label, geo::vec2 pos,
                        float container_height, frog::gx2d::Crop crop)
{
    auto& font = fonts->at(label.font);
    font.draw(*this, label, pos, container_height, crop);
}

void Renderer::draw_recursive(const RenderCtx& ctx, const gx2d::Sprite& sprite)
{
    auto draw_subsprite = [&](const gx2d::ChildSprite& sub)
    {
        RenderCtx sub_ctx = ctx;
        switch (sub.anchor.position)
        {
            case gx2d::Anchor::Position::RELATIVE:
                sub_ctx.shift      += sprite.rect.pos;
                sub_ctx.prev_shift += sprite.prev.pos;
                break;
            case gx2d::Anchor::Position::SIZE_RELATIVE:
                sub_ctx.shift      += sprite.rect.pos;
                sub_ctx.prev_shift += sprite.prev.pos;
                sub_ctx.pos_mult   *= sprite.rect.size;
                break;
            case gx2d::Anchor::Position::NONE:
                break;
        }

        if (sub.anchor.rel_size)
            sub_ctx.scale_mult *= sprite.rect.size;

        draw(sub_ctx, sub.sprite);
    };

    for (const auto& sub : sprite.children)
        if (sub.layer == gx2d::RelLayer::BELOW)
            draw_subsprite(sub);

    draw(ctx, sprite);

    for (const auto& sub : sprite.children)
        if (sub.layer == gx2d::RelLayer::ABOVE)
            draw_subsprite(sub);
}

void Renderer::draw_objects(const frog::scene_manager<frog::game_object2d>& scenes,
                            double between)
{
    // TODO: Use std::set!
    // TODO: solve in a more appropriate OOP way
    //       also, perhaps create some sort of reference counting with layers
    //       in render queue (i.e. add reference for a new game object, remove for deleted object)
    // TODO: make this more memory efficient for unhinged layer values
    //       (i.e. don't crash the game for unsigned(-1))
    std::map<unsigned, std::vector<const gx2d::Sprite*>> render_queue;

    RenderCtx ctx;
    ctx.between = between;

    std::tie(ctx.scale, ctx.shift) = scale_shift(camera);
    std::tie(ctx.prev_scale, ctx.prev_shift) = scale_shift(camera.prev);

    ctx.scale = frog::geo::lerp(ctx.prev_scale, ctx.scale, float(between));
    ctx.shift = frog::geo::lerp(ctx.prev_shift, ctx.shift, float(between));

    scenes.for_each_object([&](const auto& obj)
        {
            // // Kinda needs to be commented now.
            // if (obj.model().image_tag.empty())
            //     return;

            auto layer = obj.model().layer;

            // if (layer >= render_queue.size())
            //     render_queue.resize(layer + 1);

            render_queue[layer].push_back(&obj.model());
        });

    for (const auto& [idx, layer]: render_queue)
        for (const gx2d::Sprite* model : layer)
            draw_recursive(ctx, *model);
}

void Renderer::draw_ui(const frog::scene_manager<frog::game_object2d>& scenes,
                       double between)
{
    scenes.for_each_object([&](auto& obj)
    {
        for (const frog::ptr<gx::ui_element>& elem : obj.elements())
        {
            if (elem->hide)
                continue;

            if (not elem->sprite.image_tag.empty())
            {
                auto rect = elem->sprite.rect;

                float angle;
                gx2d::perform_interpolation(elem->sprite, between, rect, angle);

                auto tex = elem->sprite.tex;
                gx2d::apply_crop(elem->sprite, between, rect, tex);

                draw_ui_sprite(textures->at(elem->sprite.image_tag),
                            rect,
                            tex,
                            elem->sprite.color);
            }

            if (elem->label)
            {
                draw_text(*elem, between);
            }
        }
    });
}

void Renderer::draw_text(const gx::ui_element& elem, double between)
{
    if (not elem.label)
        return;

    const auto& label = *elem.label;

    auto& font = fonts->at(label.font);

    auto pos = elem.pos() + elem.label->rel_pos;

    if (auto inter = elem.sprite.interpolation; inter != gx2d::Interpolation::NONE)
    {
        float value = float(between);
        if (inter == gx2d::Interpolation::EXTRAPOLATE)
            value += 1;
        auto prev = elem.sprite.prev.pos + label.rel_pos;
        pos = frog::geo::lerp(prev, pos, value);
    }

    float container_height = elem.size().y();
    float height = container_height * label.height;
    auto text_size = font.size(label.str, height);

    if (label.align == gx::Align::CENTER)
        pos.x() -= text_size.x() / 2;
    else if (label.align == gx::Align::RIGHT)
        pos.x() -= text_size.x();
    else
        frog_assert(label.align == gx::Align::LEFT);

    gx2d::Crop crop;

    if (elem.sprite.crop)
    {
        // float dif = 0.5 * ( elem.sprite.rect.size.y() - height );
        // crop.top = elem.sprite.crop->top - dif;
        // crop.bot = elem.sprite.crop->bot - dif;

        // float width = text_size.x();
        // float x_dif = 0.5 * ( elem.sprite.rect.size.x() - width );
        // crop.left = elem.sprite.crop->left - x_dif;
        // crop.right = elem.sprite.crop->right - x_dif;

        // crop = gx2d::clamp(crop);

        auto crop_rect = geo::rect{ pos, text_size };
        crop_rect.pos.x() += text_size.x() / 2;
        crop = gx2d::multiply_crop(elem.sprite.rect, *elem.sprite.crop, crop_rect);
    }

    font.draw(*this, label, pos, height, crop);
}

#endif
