#pragma once

#include "frog/core/script.hpp"
#include "frog/gx2d/crop.hpp"
#include "frog/geometry/rectangle.hpp"


namespace frog
{

enum class button_state
{
    IDLE, HOVER, PRESS
};

template<typename GameObject>
struct button_style_base;

using button_style = button_style_base<game_object>;
using button_style2d = button_style_base<game_object2d>;

template<typename GameObject>
struct button_style_base
{
    virtual ~button_style_base() = default;

    virtual void init(GameObject&) {}

    virtual void idle(GameObject&) {}

    virtual void hover(GameObject&) {}

    virtual void press(GameObject&) {}

    virtual void activate() {}

    virtual void deactivate() {}

    virtual void stable_update(GameObject&) {}

    virtual void set_crop(GameObject&, gx2d::Crop) {}

    virtual void set_rect(GameObject&, geo::rect) {}
};


} // namespace frog
