#pragma once

#include "frog/utils/ptr.hpp"       // move


// namespace frog {
// template<typename Script>
// struct button_action_base;
// using button_action2d = button_action_base<frog::tag2d>;
// using button_action   = button_action_base<frog::tag3d>;
// template<typename Tag>
// struct button_action_base
// {
//     virtual ~button_action_base() = default;
//     virtual void action() {}
//     virtual void frame_holding() {}
//     virtual void stable_holding() {}
// };
// }       // namespace frog


namespace frog {


struct button_action
{
    virtual ~button_action() = default;

    virtual void action() {}

    virtual void frame_holding() {}

    virtual void stable_holding() {}
};


}       // namespace frog

