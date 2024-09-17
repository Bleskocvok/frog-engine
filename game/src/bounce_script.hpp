#pragma once

#include "frog/core/script.hpp"
#include "frog/geometry/paddle.hpp"

#include "constants.hpp"


struct bounce_script : frog::script
{
    frog::geo::paddle paddle;
    frog::geo::vec3 vector = { 0 };

    template<typename Constants>
    bounce_script(float angle, float y, Constants)
        : paddle(create_paddle<Constants>(angle, y)) {}

    void init(frog::game_object& object, frog::engine&) override
    {
        reset_position(object);
    }

    void stable_update(frog::game_object& object, frog::engine&) override
    {
        reset_position(object);
    }

    void move(frog::game_object& object, float angle)
    {
        paddle.pos.theta += angle;
        reset_position(object);
    }

    void move_down(frog::game_object& object, float diff)
    {
        paddle.pos.y -= diff;
        reset_position(object);
    }

    void set_y(frog::game_object& object, float y)
    {
        paddle.pos.y = y;
        reset_position(object);
    }

    void reset_position(frog::game_object& object)
    {
        object.model().rot.y() = paddle.pos.theta;
        object.model().pos.y() = paddle.pos.y;
    }

    template<typename Constants>
    static frog::geo::paddle create_paddle(float angle, float y)
    {
        auto result = frog::geo::paddle{};
        result.angular_size = Constants::AngularSize * frog::geo::ToRad;
        result.height       = Constants::Height;
        result.thickness    = Constants::Thickness;
        result.pos.r        = Constants::Radius;
        result.pos.theta    = angle * frog::geo::ToRad;
        result.pos.y        = y;
        return result;
    }
};
