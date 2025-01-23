#pragma once

#include "frog/core/script.hpp"
#include "frog/geometry/general.hpp"
#include "frog/geometry/paddle.hpp"

#include "bounce_script.hpp"
#include "ball_script.hpp"
#include "constants.hpp"


struct paddle_script : frog::script
{
    static constexpr float Strength = 3;

    bounce_script* bounce = nullptr;
    bool ball_magnet = false;

    paddle_script(bool ball_magnet = false)
        : ball_magnet(ball_magnet) {}

    void init(frog::game_object& object, frog::engine&) override
    {
        using namespace frog;

        bounce = object.get_script<bounce_script>();
    }

    void stable_update(frog::game_object& object, frog::engine& engine) override
    {
        using namespace frog;

        double time = engine.global->stable_frame_time();

        float sign = 0;

        if (engine.input->is_down(os::key{ GLFW_KEY_D }))
        {
            sign += -1;
        }
        if (engine.input->is_down(os::key{ GLFW_KEY_A }))
        {
            sign += 1;
        }

        geo::cylindrical3 new_pos = bounce->paddle.pos;
        new_pos.theta += sign * Paddle::Speed * geo::ToRad* time;

        bounce->vector = geo::to_cartesian(new_pos)
                            - geo::to_cartesian(bounce->paddle.pos);
        bounce->vector *= Strength;

        bounce->move(object, sign * Paddle::Speed * geo::ToRad * time);

        magnetize(engine);
    }

    void magnetize(frog::engine& engine)
    {
        using namespace frog;

        if (!ball_magnet)
            return;

        bool release = engine.input->is_released(os::key{ GLFW_KEY_SPACE });

        engine.scenes->current().for_each_object(frog::by_tag("ball"),
            [&](game_object& ball)
            {
                auto b_script = ball.get_script<ball_script>();

                geo::vec2 pos = geo::to_cartesian(
                    geo::polar2{
                        bounce->paddle.pos.r - Ball::Radius - geo::Epsilon,
                        bounce->paddle.pos.theta + bounce->paddle.angular_size / 2 });

               ball.model().pos.x() = pos.x();
               ball.model().pos.z() = pos.y();

                if (release)
                {
                    ball_magnet = false;
                    b_script->shoot({ -pos.x(), 0, -pos.y() });
                }
            });
    }
};
