#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/core/game_object2d.hpp"
#include "frog/geometry/circle.hpp"
#include "frog/geometry/physics.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/scripts/animation_script.hpp"
#include "frog/scripts/timer_script.hpp"

namespace frog::scripts {

class ball2d : public frog::script2d
{
    frog::geo::circle circle;
    float inv_weight = 0;
    frog::geo::soft_physics2d* physics = nullptr;
    frog::geo::soft_physics2d::idx_t idx = 0;
    frog::game_object2d* obj = nullptr;

public:
    ball2d(frog::geo::circle circle, float inv_weight, frog::geo::soft_physics2d& physics)
        : circle(circle)
        , inv_weight(inv_weight)
        , physics(&physics)
    { }

    const frog::game_object2d* object() const { return obj; }
          frog::game_object2d* object()       { return obj; }

    void init(frog::game_object2d& o, frog::engine2d& e) override
    {
        obj = &o;

        using point = frog::geo::soft_physics2d::point;
        idx = physics->add_point(point
        {
            .pos = circle.pos,
            .prev = circle.pos,
            .radius = circle.radius,
            .inv_weight = inv_weight,
        });

        obj->model().image_tag = "circle";
        obj->model().rect.pos = circle.pos;
        obj->model().rect.size = circle.radius * 2;
        obj->model().layer = 2;
        obj->model().interpolation = frog::gx2d::Interpolation::INTERPOLATE;
    }

    void end_update(frog::game_object2d& obj, frog::engine2d&) override
    {
        obj.model().rect.pos = physics->point_at(idx).pos;
    }

    void destroyed(frog::game_object2d&, frog::engine2d&) override
    {
        physics->remove_point(idx);
    }
};

} // namespace frog::scripts
