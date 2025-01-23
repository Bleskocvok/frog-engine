#pragma once

#include "frog/core/script.hpp"

#include "constants.hpp"
#include "paddle_script.hpp"

#include <string>   // std::to_string


struct player_script : frog::script
{
    static constexpr float IconSize = 0.1;
    static constexpr float Offset = 0.1;

    int lives = StartingLives;
    int score = 0;

    frog::game_object* self = nullptr;

    void init(frog::game_object& obj, frog::engine&) override
    {
        self = &obj;
        reset_ui();
    }

    void reset_ui()
    {
        using namespace frog;
        using namespace std::string_literals;

        self->elements().clear();

        float diff = lives * IconSize / 2;

        for (int i = 0; i < lives; i++)
        {
            auto* heart = self->add_element(mk_ptr<gx::ui_element>());
            heart->sprite.image_tag = "heart";
            heart->pos() = { i * IconSize - diff,
                           1 - IconSize };
            heart->size() = { IconSize };
            heart->tex_pos() = { 0, 0 };
        }

        auto* display = self->add_element(mk_ptr<gx::ui_element>());
        display->label = { "score: "s + std::to_string(score) };
        display->label->centered = true;
        display->color() = gx::color_to_255({ 0.3, 0.3, 0.8, 1 });
        display->pos() = { 0, 1 - IconSize - Offset };
        display->size() = { IconSize * 0.75 };
    }

    void add_lives(int val)
    {
        lives += val;
        reset_ui();
    }

    void add_score(int val)
    {
        score += val;
        reset_ui();
    }

    bool dead() const
    {
        return lives <= 0;
    }
};
