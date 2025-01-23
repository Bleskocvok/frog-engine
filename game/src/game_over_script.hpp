#pragma once

#include "frog/core/script.hpp"
#include "frog/geometry/general.hpp"
#include "frog/geometry/paddle.hpp"

#include "bounce_script.hpp"
#include "constants.hpp"
#include "player_script.hpp"

#include <string>



struct game_over_script : frog::script
{
    static constexpr float TextSize = 0.2;

    frog::gx::ui_element* score = nullptr;

    void init(frog::game_object& object, frog::engine&) override
    {
        using namespace frog;

        auto* text = object.add_element(mk_ptr<gx::ui_element>());
        text->label = { "Game Over" };
        text->pos() = { 0 };
        text->size() = { TextSize };
        text->label->centered = true;

        score = object.add_element(mk_ptr<gx::ui_element>());
        score->label = { "score" };
        score->label->centered = true;
        score->pos() = { 0, 0 - text->size().y() };
        score->size() = { TextSize * 0.5 };
    }

    void stable_update(frog::game_object&, frog::engine& engine) override
    {
        engine.scenes->at("main")
            .for_each_script<player_script>(
            [&](const player_script& pl_scr)
            {
                score->label->str = "score: " + std::to_string(pl_scr.score);
            });
    }
};
