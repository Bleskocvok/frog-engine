#pragma once

#include "core/script.hpp"
#include "core/engine.hpp"
#include "geometry/general.hpp"
#include "geometry/paddle.hpp"
#include "geometry/polar.hpp"
#include "utils/debug.hpp"

#include "constants.hpp"

#include <string>


enum class powerup_type
{
    Life, Score, SpeedUp, SpeedDown, Count
};


struct powerup_script : frog::script
{
    static constexpr float Speed = 1.5;
    static constexpr float SpeedDown = 0.666;
    static constexpr float SpeedUp = 1.1;
    static constexpr int ScoreAmount = 5;

    powerup_type type;
    frog::geo::cylindrical3 coord;

    powerup_script(powerup_type type, frog::geo::cylindrical3 coord)
        : type(type), coord(coord) {}

    void init(frog::game_object& object, frog::engine&) override;

    void stable_update(frog::game_object& object, frog::engine& engine) override
    {
        using namespace frog;

        check_collision(object, engine);

        if (coord.r > GroundSize)
        {
            object.destroy();
        }

        coord.r += Speed * engine.global->stable_frame_time();
        object.model().pos = geo::to_cartesian(coord);
    }

    void check_collision(frog::game_object& object, frog::engine& engine);

    void apply_effect(frog::engine& engine);
};

