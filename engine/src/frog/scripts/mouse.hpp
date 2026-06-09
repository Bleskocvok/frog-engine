#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"

namespace frog::scripts {

class Mouse : public frog::script2d
{
public:
    void init(frog::game_object2d& obj, frog::engine2d& e) override
    {}
};

} // namespace frog::scripts
