#pragma once

#include "frog/gx2d/sprite.hpp"

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

} // namespace frog
