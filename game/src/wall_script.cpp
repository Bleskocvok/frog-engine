#include "wall_script.hpp"

#include "core/engine3d.hpp"

#include "player_script.hpp"


void wall_script::destroyed(frog::game_object&, frog::engine& engine)
{
    using namespace frog;

    auto broadcast = [&](game_object& wall)
    {
        auto* script = wall.get_script<wall_script>();
        if (script)
        {
            script->wall_destroyed(wall, element, level);
        }
    };

    engine.scenes->current()
        .for_each_object(frog::by_tag("wall"), broadcast);

    engine.scenes->current()
        .for_each_script<player_script>(
            [&](auto& pl_scr) { pl_scr.add_score(durability); });

    create_powerup(engine);
}
