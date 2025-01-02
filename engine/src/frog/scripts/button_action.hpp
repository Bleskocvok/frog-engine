#pragma once


namespace frog {


struct button_action
{
    virtual ~button_action() = default;

    virtual void action() {}

    virtual void frame_holding() {}

    virtual void stable_holding() {}
};


}       // namespace frog

