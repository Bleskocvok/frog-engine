#include "powerup_script.hpp"

#include "bounce_script.hpp"
#include "player_script.hpp"
#include "ball_script.hpp"


void powerup_script::init(frog::game_object& object, frog::engine&)
{
    using namespace frog;

    switch (type)
    {
    case powerup_type::Life:
        object.model().mat.diffuse = { 0.8, 0.2, 0.2, 1 };
        break;
    case powerup_type::Score:
        object.model().mat.diffuse = { 0.2, 0.2, 0.8, 1 };
        break;
    case powerup_type::SpeedUp:
        object.model().mat.diffuse = { 0.05, 0.36, 0.1, 1 };
        break;
    case powerup_type::SpeedDown:
        object.model().mat.diffuse = { 0.2, 0.8, 0.2, 1 };
        break;
    default:
        break;
    }

    object.model().mat.specular = { 1, 1, 1, 120 };
    object.model().mat.ambient = object.model().mat.diffuse;

    coord.y = PowerupSize;
    object.model().pos = geo::to_cartesian(coord);
}


void powerup_script::check_collision(frog::game_object& object, frog::engine& engine)
{
    using namespace frog;

    engine.scenes->current().for_each_object(by_tag("paddle"),
        [&](game_object& paddle)
    {
        auto* bounce = paddle.get_script<bounce_script>();

        if (bounce && geo::collision_2D(bounce->paddle, coord, PowerupSize))
        {
            object.destroy();
            apply_effect(engine);
        }
    });
}

void powerup_script::apply_effect(frog::engine& engine)
{
    switch (type)
    {
        case powerup_type::Life:
            engine.scenes->current()
                .for_each_script<player_script>([&](auto& pl_sc)
            {
                pl_sc.add_lives(1);
            });
            break;
        case powerup_type::Score:
            engine.scenes->current()
                .for_each_script<player_script>([&](auto& pl_sc)
            {
                pl_sc.add_score(ScoreAmount);
            });
            break;
        case powerup_type::SpeedUp:
            engine.scenes->current()
                .for_each_script<ball_script>([&](auto& ball)
            {
                ball.multiply_speed(SpeedUp);
            });
            break;
        case powerup_type::SpeedDown:
            engine.scenes->current()
                .for_each_script<ball_script>([&](auto& ball)
            {
                ball.multiply_speed(SpeedDown);
            });
            break;
        default:
            break;
    }
}

