#pragma once

#include "frog/core/script.hpp"
#include "frog/core/engine2d.hpp"
#include "frog/geometry/vector.hpp"
#include "frog/graphics/color.hpp"
#include "frog/graphics/ui_element.hpp"
#include "frog/gx2d/sprite.hpp"
#include "frog/scripts/profiler_script.hpp"
#include "frog/utils/string_builder.hpp"

#include <sstream>
#include <string>
#include <utility>      // move
#include <vector>

namespace frog::scripts {

class ProfilerDisplay : public frog::script2d
{
    frog::geo::vec2 start;

    std::vector<gx::ui_element*> elems;

public:
    explicit ProfilerDisplay(frog::geo::vec2 start = {})
        : start(start)
    { }

    void frame_update(frog::game_object2d& obj, frog::engine2d& e) override
    {
        for (auto* elem : elems)
            obj.remove_element(elem);

        static constexpr float label_height = 0.03;
        auto pos = start;

        auto* profiler = e.scenes->current().get_script<frog::scripts::ProfilerScript>();
        if (not profiler)
            return;

        {
            auto* bg = obj.add_element(frog::mk_ptr<frog::gx::ui_element>());
            elems.push_back(bg);

            bg->sprite.image_tag = "box";
            bg->sprite.color = { 0, 0, 0, 128 };

            bg->sprite.rect.size = { 1, label_height * profiler->times().size() * 2 };
            bg->sprite.rect.pos = start;
            bg->sprite.rect.pos.y() += 0.5 * label_height * profiler->times().size();
        }

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
            ui->label->str = frog::make_string("|", std::move(o).str());

            pos.y() += 0.025;
        }
    }
};

} // namespace frog::scripts
