#pragma once

#include "frog/gx2d/sprite.hpp"
#include "frog/geometry/rectangle.hpp"

#include <algorithm>
#include <string>
#include <tuple>        // tie, tuple
#include <vector>

namespace frog {

inline gx2d::Sprite build_frame(frog::geo::rect rect,
                 const std::string& image_tag,
                 frog::geo::vec2 tex_size = { 5 / 32.0, 5 / 16.0 },
                 frog::geo::vec2 elem_size = { 0.02, 0.02 },
                 float around = 0)
{
    using namespace frog::geo;

    rect.size -= elem_size;
    rect.size += around * elem_size * 2;

    gx2d::Sprite parent;
    parent.children.resize(8);

    // Clockwise: 0 1 2 3 4 5 6 7
    //            ↑ ↗ → ↘ ↓ ↙ ← ↖
    auto& t  = parent.children.at(0);
    auto& b  = parent.children.at(1);
    auto& l  = parent.children.at(2);
    auto& r  = parent.children.at(3);
    auto& tl = parent.children.at(4);
    auto& tr = parent.children.at(5);
    auto& bl = parent.children.at(6);
    auto& br = parent.children.at(7);

    for (auto* elem : { &t, &b, &l, &r, &tl, &tr, &bl, &br })
    {
        elem->sprite.image_tag = image_tag;
        elem->sprite.rect.size = elem_size;
        elem->sprite.tex.size = tex_size;
    }

    t.sprite.rect.pos = 0.5 * ( rect.top_left() + rect.top_right() );
    t.sprite.rect.size = { rect.size.x() - elem_size.x(), elem_size.y() };
    t.sprite.tex.size = { 1 - tex_size.x() * 2, tex_size.y() };
    t.sprite.tex.pos = { tex_size.x(), 0 };

    b.sprite.rect.pos = 0.5 * ( rect.bot_left() + rect.bot_right() );
    b.sprite.rect.size = { rect.size.x() - elem_size.x(), elem_size.y() };
    b.sprite.tex.size = { 1 - tex_size.x() * 2, tex_size.y() };
    b.sprite.tex.pos = { tex_size.x(), 1 - tex_size.y() };

    l.sprite.rect.pos = 0.5 * ( rect.top_left() + rect.bot_left() );
    l.sprite.rect.size = { elem_size.x(), rect.size.y() - elem_size.y() };
    l.sprite.tex.size = { tex_size.x(), 1 - tex_size.y() * 2 };
    l.sprite.tex.pos = { 0, tex_size.y() };

    r.sprite.rect.pos = 0.5 * ( rect.top_right() + rect.bot_right() );
    r.sprite.rect.size = { elem_size.x(), rect.size.y() - elem_size.y() };
    r.sprite.tex.size = { tex_size.x(), 1 - tex_size.y() * 2 };
    r.sprite.tex.pos = { 1 - tex_size.x(), tex_size.y() };

    tl.sprite.rect.pos = rect.top_left();

    tr.sprite.rect.pos = rect.top_right();
    tr.sprite.tex.pos = { 1 - tex_size.x(), 0 };

    bl.sprite.rect.pos = rect.bot_left();
    bl.sprite.tex.pos = { 0, 1 - tex_size.y() };

    br.sprite.rect.pos = rect.bot_right();
    br.sprite.tex.pos = { 1 - tex_size.x(), 1 - tex_size.y() };

    return parent;
}

struct PanelTextureInfo
{
    std::string image_tag;
    int top = 0;
    int bot = 0;
    int left = 0;
    int right = 0;
    frog::geo::ivec2 tex_size;
};

inline frog::gx2d::Sprite make_pixel_panel(const PanelTextureInfo& info,
                                           frog::geo::irect irect_,
                                           frog::geo::rect rect = {})
{
    frog::gx2d::Sprite parent;

    auto pixels_to_uv = [](frog::geo::irect pixels, frog::geo::ivec2 isize) -> frog::geo::rect
    {
        frog_assert(isize.x() != 0);
        frog_assert(isize.y() != 0);

        auto unit = frog::geo::vec2{ 1.0f / isize.x(), 1.0f / isize.y() };

        frog::geo::rect rect;
        rect.size = frog::geo::vec2{ pixels.size.x() * unit.x(),
                                     pixels.size.y() * unit.y() };

        rect.pos = frog::geo::vec2{ pixels.pos.x() * unit.x(),
                                    pixels.pos.y() * unit.y() };

        return rect;
    };

    auto half_l = irect_.size.x() / 2;
    auto half_r = irect_.size.x() - half_l;
    auto half_t = irect_.size.y() / 2;
    auto half_b = irect_.size.y() - half_t;

    int itop   = std::min(info.top,   half_t);
    int ibot   = std::min(info.bot,   half_b);
    int ileft  = std::min(info.left,  half_l);
    int iright = std::min(info.right, half_r);

    int iborder_width = std::min( irect_.size.x() - ileft - iright,
                                  info.tex_size.x() - info.left - info.right );
    int iborder_height = std::min( irect_.size.y() - itop - ibot,
                                  info.tex_size.y() - info.top - info.bot );

    using namespace frog::geo;

    auto float_size = frog::geo::vec2{ static_cast<float>(irect_.size.x()),
                                       static_cast<float>(irect_.size.y()) };

    auto rect_tex = [&](int x, int y, int w, int h, int u, int v)
        -> std::tuple<frog::geo::rect, frog::geo::rect>
    {
        frog::geo::rect tex  = pixels_to_uv(irect( u, v, w, h ), info.tex_size);
        frog::geo::rect rect = pixels_to_uv(irect( x, y, w, h ), irect_.size);
        rect.pos += -0.5 + rect.size / 2;
        return { tex, rect };
    };

    const auto data = std::vector<std::tuple<int, int, int, int, int, int>>
    {
        // top right
        { ileft + iborder_width, 0, iright, itop, info.tex_size.x() - iright, 0 },
        // bot righte
        { ileft + iborder_width, itop + iborder_height, iright, ibot, info.tex_size.x() - iright, info.tex_size.y() - ibot },
        // bot left
        { 0, itop + iborder_height, ileft, ibot, 0, info.tex_size.y() - ibot },
        // top left
        { 0, 0, ileft, itop, 0, 0 },
        // top
        { ileft, 0, iborder_width, itop, ileft, 0 },
        // right
        { ileft + iborder_width, itop, iright, iborder_height, info.tex_size.x() - iright, itop },
        // bot
        { ileft, itop + iborder_height, iborder_width, ibot, ileft, info.tex_size.y() - ibot },
        // left
        { 0, itop, iright, iborder_height, 0, itop },
        // center
        { ileft, itop, iborder_width, iborder_height, ileft, itop },
    };

    for (auto [x, y, w, h, u, v] : data)
    {
        if (w <= 0 || h <= 0)
            continue;

        auto& child = parent.children.emplace_back();
        std::tie(child.sprite.tex, child.sprite.rect) = rect_tex( x, y, w, h, u, v );

        child.sprite.image_tag = info.image_tag;
        child.anchor.position = frog::gx2d::Anchor::Position::SIZE_RELATIVE;
        child.anchor.rel_size = true;
        child.anchor.rel_angle = decltype(child.anchor.rel_angle)::TRANSLATE;
    }

    parent.rect = rect;

    return parent;
}

} // namespace frog
