#pragma once

#include "core/script.hpp"
#include "geometry/basic.hpp"


struct camera_script : frog::script
{
    void init(frog::game_object&, frog::engine& engine) override
    {
        using namespace frog;

        switch_camera(engine, 1);
    }

    void stable_update(frog::game_object&, frog::engine& engine) override
    {
        using namespace frog;

        if (engine.input->is_pressed(os::key{ GLFW_KEY_1 }))
        {
            switch_camera(engine, 1);
        }

        if (engine.input->is_pressed(os::key{ GLFW_KEY_2 }))
        {
            switch_camera(engine, 2);
        }
    }

    void switch_camera(frog::engine& engine, int mode)
    {
        using namespace frog;

        switch (mode)
        {
            case 1:
                engine.camera.look_towards({ 0, 0, 0 }, 10, 0, geo::Pi / 4);
                break;
            case 2:
                engine.camera.look_towards({ 0, 0, 0 }, 10, 0, geo::Pi / 2);
                break;
            default:
                break;
        }
    }
};
