#pragma once

#include "core/script.hpp"
#include "core/engine.hpp"
#include "core/object_builder.hpp"
#include "geometry/paddle.hpp"

#include "constants.hpp"
#include "bounce_script.hpp"
#include "powerup_script.hpp"

#include <optional>


struct wall_script : frog::script
{
    static constexpr float Gravity = 2;
    inline static const frog::geo::vec4 dark = { 0.2, 0.2, 0.2, 1 };

    int element;
    int level;
    int durability;
    int dmg = 0;

    bounce_script* bounce = nullptr;
    frog::gl::material default_mat;

    struct fall
    {
        float time = 0;
        int levels = 1;
    };

    std::optional<fall> falling = std::nullopt;
    std::optional<powerup_type> powerup = std::nullopt;


    wall_script(int element,
                int level,
                int durability,
                std::optional<powerup_type> powerup = std::nullopt)
        : element(element)
        , level(level)
        , durability(durability)
        , powerup(std::move(powerup)) {}

    void init(frog::game_object& object, frog::engine&) override
    {
        using namespace frog;

        bounce = object.get_script<bounce_script>();
        default_mat = object.model().mat;

        object.model().mat.diffuse -= dark * durability;
    }

    void stable_update(frog::game_object& object, frog::engine& engine) override
    {
        if (falling)
        {
            falling->time += engine.global->stable_frame_time();
            float new_y = level * Wall::Height - falling->time * falling->time * Gravity;

            if (new_y <= (level - falling->levels) * Wall::Height)
            {
                level -= falling->levels;
                new_y = level * Wall::Height;
                falling.reset();
            }

            bounce->set_y(object, new_y);
        }
    }

    void destroyed(frog::game_object&, frog::engine& engine) override;

    void create_powerup(frog::engine& engine)
    {
        if (!powerup)
            return;

        frog::object_builder builder;
        builder.add_mesh("powerup");

        auto pos = bounce->paddle.pos;
        pos.theta += bounce->paddle.angular_size / 2;
        builder.emplace_script<powerup_script>(powerup.value(), pos);

        builder.build_in(engine);
    }

    void damage(frog::game_object& obj)
    {
        dmg++;

        auto a = obj.model().mat.diffuse;
        auto b = default_mat.diffuse;
        float val = dmg / static_cast<float>(durability);

        obj.model().mat.diffuse = (1 - val) * a + val * b;

        if (dmg >= durability)
        {
            obj.destroy();
        }
    }

    void wall_destroyed(frog::game_object& obj, int el, int lvl)
    {
        // check if the destroyed wall was below us
        if (el == element && lvl < level)
        {
            if (falling)
                falling->levels++;
            else
                falling = { 0.f, 1 };
        }
    }
};
