#pragma once

#include "core/script.hpp"
#include "utils/debug.hpp"

#include "constants.hpp"
#include "paddle_script.hpp"
#include "bounce_script.hpp"
#include "wall_script.hpp"
#include "player_script.hpp"

#include <random>


struct game_script : frog::script
{
    std::mt19937 random;

    game_script()
        : random(std::random_device{}()) {}

    void init(frog::game_object&, frog::engine& engine) override
    {
        add_walls(engine);
    }

    void stable_update(frog::game_object&, frog::engine& engine) override
    {
        using namespace frog;

        engine.scenes->current().for_each_object(by_tag("ball"),
            [&](auto& ball)
        {
            if (distance(ball.model().pos, geo::vec3{ 0 }) > GroundSize)
            {
                reset_ball(ball, engine);
                remove_player_health(engine);
            }
        });

        check_all_walls_destroyed(engine);
    }

    void reset_ball(frog::game_object& ball, frog::engine& engine)
    {
        ball.model().pos = { 0, Ball::Radius, 0 };

        int count = 0;
        engine.scenes->current()
            .for_each_script<paddle_script>(frog::by_tag("paddle"),
                [&](const auto&) { count++; });

        int i = 0;
        engine.scenes->current()
            .for_each_script<paddle_script>(
                frog::by_tag("paddle"),
                [&](paddle_script& sc)
                {
                    sc.ball_magnet = i == 0;
                    i++;
                });
    }

    void remove_player_health(frog::engine& engine)
    {
        engine.scenes->current()
            .for_each_script<player_script>(
                [&](auto& pl_script)
        {
            pl_script.add_lives(-1);
            if (pl_script.dead())
            {
                game_over(engine);
            }
        });
    }

    void game_over(frog::engine& engine)
    {
        engine.scenes->switch_scene("game_over");
    }

    void check_all_walls_destroyed(frog::engine& engine)
    {
        bool walls_destroyed = true;
        engine.scenes->current().for_each_object(frog::by_tag("wall"),
            [&](auto&) { walls_destroyed = false; });

        // if all have been destroyed, restart them
        if (walls_destroyed)
        {
            add_walls(engine);
            reset_ball(*engine.scenes->current()
                .get_object(frog::by_tag("ball")), engine);
        }
    }

    std::optional<powerup_type> generate_powerup()
    {
        using distribution = std::uniform_int_distribution<int>;

        distribution dist{ 0, 100 };

        if (dist(random) > 30)
            return std::nullopt;

        int max = static_cast<int>(powerup_type::Count) - 1;

        distribution pow_dist(0, max);
        auto type = powerup_type(pow_dist(random));

        if (type == powerup_type::Life
                && distribution{ 0, 1 }(random) == 0)
        {
            return std::nullopt;
        }

        return type;
    }

    void add_walls(frog::engine& eng)
    {
        using namespace frog;

        object_builder builder;

        auto wall_mats = std::array
        {
            gl::material
            {
                geo::vec4{ 0.12, 0.16, 0.08, 1 },
                geo::vec4{ 252, 166, 86, 1 } / 255.f,
                geo::vec4{ 1, 1, 1, 40 },
            },
            gl::material
            {
                geo::vec4{ 0.12, 0.16, 0.08, 1 },
                geo::vec4{ 255, 255, 171, 1 } / 255.f,
                geo::vec4{ 1, 1, 1, 40 },
            },
        };

        float alpha = Wall::AngularSize / 2;

        int idx = 0;
        for (int i = 0; i < Wall::Levels; i++)
        {
            int mat_idx = 0;
            for (int j = 0; j < Wall::NumberPerLevel; j++)
            {
                int durability = idx % 5 == 0 ? 4 : 1;
                builder.reset();
                builder.add_mesh("wall");
                builder.set_tag("wall");
                builder.set_material(wall_mats[(i + mat_idx) % wall_mats.size()]);
                builder.emplace_script<wall_script>(j,
                                                    i,
                                                    durability,
                                                    generate_powerup());
                builder.emplace_script<bounce_script>(j * Wall::AngularSize + alpha,
                                                      i * Wall::Height,
                                                      Wall{});
                builder.build_in(eng);

                mat_idx++;
                idx++;
            }
        }
    }
};
