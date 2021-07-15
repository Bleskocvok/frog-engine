#pragma once

#include "game_object.hpp"


namespace frog
{


class game_object;
class engine;


struct script
{
    virtual ~script() = default;

    virtual void init(game_object&, engine&) {}

    virtual void stable_update(game_object&, engine&) {}

    virtual void end_update(game_object&, engine&) {}

    virtual void frame_update(game_object&, engine&) {}

    virtual void destroyed(game_object&, engine&) {}
};


} // namespace frog
