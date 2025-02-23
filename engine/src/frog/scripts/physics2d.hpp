#pragma once

#include "frog/geometry/vector.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/utils/string_builder.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/geometry/physics.hpp"
#include "frog/geometry/basic.hpp"
#include "frog/geometry/transform.hpp"
#include "frog/geometry/circle.hpp"

namespace frog {

class physics_script2d : frog::script2d
{
    frog::geo::soft_physics2d physics_ =
    {
        { .grid_dim = { 10, 10 } },
        decltype(physics_)::limits{ 20000, 20000, 200 }
    };

public:
    explicit physics_script2d(frog::geo::soft_physics2d physics)
        : physics_(std::move(physics)) {}

    void init(frog::game_object2d& object, frog::engine2d&) override
    {
        using namespace frog;
    }

    void stable_update(frog::game_object2d&, frog::engine2d& engine) override
    {
        physics_.update();
    }

    const frog::geo::soft_physics2d& physics() const { return physics_; }
          frog::geo::soft_physics2d& physics()       { return physics_; }

    frog::geo::soft_physics2d::idx_t add_point(frog::geo::soft_physics2d::point pt);
};

} // namespace frog
