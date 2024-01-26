#ifndef NOT_FROG_BUILD_2D

#pragma once

#include "game_object_base.hpp"


namespace frog
{


class engine2d;
struct script2d;


class game_object2d : public game_object_base<game_object2d, script2d, engine2d>
{
public:
    using Engine = engine2d;

private:

public:
    game_object2d() = default;
};


} // namespace frog


#endif
