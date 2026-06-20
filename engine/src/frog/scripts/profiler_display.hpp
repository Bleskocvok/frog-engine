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

        auto pos = start;

        mk_bg(obj, profiler);

        for (const auto& t : profiler->times())
        {
            auto* ui = obj.add_element(frog::mk_ptr<frog::gx::ui_element>());
            elems.push_back(ui);

            ui->label = { "text:", 1 };
            ui->label->align = decltype(ui->label->align)::CENTER;
            ui->pos() = pos;
            ui->size() = { 0, label_height };
            // TODO: Fuck.
            ui->sprite.layer = 100000;

            std::ostringstream o;
            profiler->out_line(o, t);
            ui->label->str = frog::make_string(std::move(o).str());

            pos.y() += 0.025;
        }
    }

    void mk_bg(frog::game_object2d& obj, frog::scripts::ProfilerScript* profiler)
    {
        auto count = profiler ? profiler->times().size() : 5;
        count = std::max(decltype(count)(5), count);

        auto* bg = obj.add_element(frog::mk_ptr<frog::gx::ui_element>());
        elems.push_back(bg);

        bg->sprite.image_tag = "box";
        bg->sprite.color = { 0, 0, 0, 128 };

        bg->sprite.rect.size = { 0.6, label_height * count * 2 };
        bg->sprite.rect.pos = start;
        bg->sprite.rect.pos.y() += 0.5 * label_height * count;
    }
};

} // namespace frog::scripts
