#pragma once

#include "frog/core/script.hpp"

#include "constants.hpp"
#include "wall_script.hpp"
#include "bounce_script.hpp"

#include <vector>
#include <optional>
#include <algorithm>    // std::remove_if


struct speed_change
{
    float value = 0;

    float duration = 1; // in seconds
    float time = 0;

    speed_change(float value, float duration)
        : value(value)
        , duration(duration) {}

    bool ended() const { return time >= duration; }

    void update(float& speed, float delta)
    {
        time += delta;
        speed += delta * value / duration;
    }
};


struct ball_script : frog::script
{
    frog::geo::vec3 vector{ 0 };
    float base_speed = Ball::DefaultSpeed;
    float current_speed = base_speed;
    float time_to_increment = 0;

    std::vector<speed_change> changes;

    void init(frog::game_object& obj, frog::engine&) override
    {
        using namespace frog;

        obj.model().pos = { 0, Ball::Radius, 0 };
    }

    void stable_update(frog::game_object& obj, frog::engine& engine) override
    {
        using namespace frog;

        auto prev_pos = obj.model().pos;

        obj.model().pos += engine.global->stable_frame_time()
                            * current_speed
                            * vector;

        float radius = Ball::Radius;
        auto polar_pos = geo::to_cylindrical(obj.model().pos);
        bool bounced = false;

        auto bounce = [&](game_object& wall)
        {
            if (bounced)
                return;

            auto found = wall.get_script<bounce_script>();

            if (!found)
                return;

            auto paddle = found->paddle;

            auto collision = geo::collision_2D(paddle, polar_pos, radius);

            if (collision)
            {
                bounced = true;

                float len = vector.length();
                vector = geo::reflect(vector, collision->normal);
                vector += found->vector / current_speed;
                vector.normalize() *= len;

                obj.model().pos = collision->resolved;

                auto wall_sc = wall.get_script<wall_script>();
                if (wall_sc)
                {
                    wall_sc->damage(wall);
                }
            }
        };

        engine.scenes->current()
            .for_each_object(frog::by_tag("paddle"), bounce);

        engine.scenes->current()
            .for_each_object(frog::by_tag("wall"), bounce);

        update_speed(obj, engine);
        speedup_area(prev_pos, obj, engine);
    }

    // solve periodical speed increase
    void update_speed(frog::game_object&, frog::engine& engine)
    {
        using namespace frog;
        float frame_time = engine.global->stable_frame_time();

        time_to_increment += frame_time;
        if (time_to_increment > Ball::IncrementPeriod)
        {
            time_to_increment -= Ball::IncrementPeriod;
            multiply_speed(Ball::IncrementSpeed);
        }
    }

    void multiply_speed(float coeff)
    {
        base_speed *= coeff;
        current_speed *= coeff;
    }

    // solve speedup area in the middle of the play area
    void speedup_area(frog::geo::vec3 prev_pos,
                      frog::game_object& obj,
                      frog::engine& engine)
    {
        using namespace frog;
        float frame_time = engine.global->stable_frame_time();

        auto current = obj.model().pos;

        auto inside = [](geo::vec3 pos)
        {
            return pos.length_squared() < std::pow(Ball::SpeedupArea, 2);
        };


        if (inside(prev_pos) != inside(current))
        {
            float growth = inside(current) ? 1 : -1;
            changes.emplace_back(Ball::SpeedupValue * growth,
                                 Ball::SpeedupDuration);
        }

        for (auto& change : changes)
        {
            change.update(current_speed, frame_time);
        }
        changes.erase(std::remove_if(changes.begin(), changes.end(),
                        [](const auto& ch) { return ch.ended(); }),
                      changes.end());
    }

    void shoot(frog::geo::vec3 normal)
    {
        vector = normal;
        vector.normalize();
    }

    void frame_update(frog::game_object&, frog::engine&) override {}

    void destroyed(frog::game_object&, frog::engine&) override {}
};
