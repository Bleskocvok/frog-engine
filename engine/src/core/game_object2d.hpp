#ifndef NOT_FROG_BUILD_2D

#pragma once

#include "game_object_base.hpp"
#include "gx2d/sprite.hpp"


namespace frog
{


class engine2d;
struct script2d;


class game_object2d : public game_object_base<game_object2d, script2d, engine2d>
{
public:
    using Engine = engine2d;

private:
    gx2d::sprite img;

public:
    game_object2d() = default;

    const gx2d::sprite& model() const { return img; }
          gx2d::sprite& model()       { return img; }
};


} // namespace frog


#endif
