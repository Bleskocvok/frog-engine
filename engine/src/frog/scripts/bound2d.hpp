#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/core/game_object2d.hpp"
#include "frog/geometry/circle.hpp"
#include "frog/geometry/general.hpp"
#include "frog/geometry/physics.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/graphics/ui_element.hpp"

#include <cmath>
#include <optional>
#include <utility>      // move

namespace frog::scripts {

class Bound2d : public frog::script2d
{
    frog::geo::soft_physics2d* physics = nullptr;
    frog::geo::soft_physics2d::idx_t idx = -1;
    frog::geo::soft_physics2d::idx_t idx_add = -1;

    frog::geo::soft_physics2d::idx_t idx_rot_a = -1;
    frog::geo::soft_physics2d::idx_t idx_rot_b = -1;

    float rotation_delta_deg = 0;

    frog::gx::ui_element* ui = nullptr;

    gx2d::Sprite& sprite(frog::game_object2d& obj)
    {
        return ui == nullptr ? obj.model() : ui->sprite;
    }

public:
    Bound2d(frog::geo::soft_physics2d& physics, frog::gx::ui_element* ui = nullptr)
        : physics(&physics)
        , ui(ui)
    { }

    void bind(frog::geo::soft_physics2d::idx_t i)
    {
        idx = i;
    }

    void bind_between(frog::geo::soft_physics2d::idx_t a,
                      frog::geo::soft_physics2d::idx_t b)
    {
        idx = a;
        idx_add = b;
    }

    void bind_rotation(frog::geo::soft_physics2d::idx_t a,
                       frog::geo::soft_physics2d::idx_t b)
    {
        idx_rot_a = a;
        idx_rot_b = b;
    }

    void rotation_delta(float deg)
    {
        rotation_delta_deg = deg;
    }

    void init(frog::game_object2d&, frog::engine2d&) override
    {
    }

    void end_update(frog::game_object2d& obj, frog::engine2d&) override
    {
        if (idx_add != -1 && idx != -1)
        {
            const auto& a = physics->point_at(idx);
            const auto& b = physics->point_at(idx_add);

            sprite(obj).rect.pos = frog::geo::lerp(a.pos, b.pos, 0.5f);
            sprite(obj).angle = geo::angle_deg(a.pos, b.pos);
        }
        else if (idx != -1)
            sprite(obj).rect.pos = physics->point_at(idx).pos;

        if (idx_rot_a != -1 && idx_rot_b != -1)
        {
            const auto& a = physics->point_at(idx);
            const auto& b = physics->point_at(idx_add);
            sprite(obj).angle = rotation_delta_deg + geo::angle_deg(a.pos, b.pos);
        }
    }
};

} // namespace frog::scripts
