#pragma once


namespace frog {


struct engine2d;
struct engine;
struct game_object2d;
struct game_object;


#ifndef NOT_FROG_BUILD_3D

struct tag3d
{
    using Engine = engine;
    using GameObject = game_object;
};

#endif


#ifndef NOT_FROG_BUILD_2D

struct tag2d
{
    using Engine = engine2d;
    using GameObject = game_object2d;
};

#endif


}       // namespace frog
