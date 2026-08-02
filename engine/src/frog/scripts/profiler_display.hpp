#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/scripts/profiler_script.hpp"
#include "frog/utils/string_builder.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <utility>      // move
#include <vector>

namespace frog::scripts {

class ProfilerDisplay : public frog::script2d
{
    static constexpr float label_height = 0.03;

    frog::geo::vec2 start;

    std::vector<gx::ui_element*> elems;

    void mk_bg(frog::game_object2d& obj, frog::scripts::ProfilerScript* profiler);

    void mk_labels(frog::game_object2d& obj, frog::scripts::ProfilerScript* profiler);

    struct Ctx
    {
        int indent = 0;
        frog::game_object2d& obj;
        frog::geo::vec2 pos;
        frog::scripts::ProfilerScript* profiler;
    };

    void mk_one(Ctx& ctx, const ProfilerGuard::Key& key, const auto& node);

public:
    explicit ProfilerDisplay(frog::geo::vec2 start = {})
        : start(start)
    { }

    void init(frog::game_object2d& obj, frog::engine2d&) override
    {
        mk_bg(obj, nullptr);
    }

    void frame_update(frog::game_object2d& obj, frog::engine2d& e) override
    {
        auto* profiler = e.scenes->current().get_script<frog::scripts::ProfilerScript>();
        if (not profiler)
            return;

        if (not profiler->changed())
            return;

        for (auto* elem : elems)
            obj.remove_element(elem);

        // TODO: Optimize this shit out of this crap.
        mk_bg(obj, profiler);

        mk_labels(obj, profiler);
    }
};

} // namespace frog::scripts
