#pragma once

#include "core/script.hpp"


struct quit_script : frog::script
{
    int key = 0;

    quit_script(int key)
        : key(key) {}

    void stable_update(frog::game_object&, frog::engine& engine) override
    {
        using namespace frog;

        if (engine.input->is_pressed(os::key{ key }))
        {
            engine.global->quit = true;
        }
    }
};
