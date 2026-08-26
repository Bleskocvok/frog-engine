#include "profiler_script.hpp"

namespace frog::scripts {

void ProfilerScript::stable_update(frog::game_object2d&, frog::engine2d&)
{
    changed_announce = false;

    if (emit)
    {
        emit = false;

        changed_announce = true;
    }
}

void ProfilerScript::frame_update(frog::game_object2d&, frog::engine2d& e)
{
    accum += e.global->frame_time();

    for (const auto&[key, us] : ProfilerGuard::times())
        times_us[key] += us;

    ProfilerGuard::reset();

    if (accum >= 1)
    {
        accum -= 1;

        // print();
        snapshot = times_us;
        emit = true;

        times_us.clear();
    }
}

} // namespace frog::scripts
