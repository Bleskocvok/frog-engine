#pragma once

#include "graphics/renderer_base.hpp"
#include "graphics/assets.hpp"

#include "os/window_base.hpp"
#include "os/input.hpp"
#include "os/timer.hpp"

#include "utils/ptr.hpp"

#include "scene_manager.hpp"
#include "settings.hpp"
#include "state.hpp"
#include "script.hpp"

#include <string_view>
#include <utility>      // move


namespace frog {


// Derived = CRTP
template<typename Derived, typename GameObject, typename Timer>
class engine_base
{
     const Derived& get() const  { return *static_cast<const Derived*>(this); }
           Derived& get()        { return *static_cast<      Derived*>(this); }

protected:
    ptr<os::window_base> window;
    ptr<gx::renderer_base> renderer;

    virtual void init() = 0;

    virtual void draw_objects(double between) = 0;
    virtual void draw_ui(double between) = 0;

    virtual void update_controls() = 0;
    virtual void frame_update() { scenes->frame_update(get()); }

    virtual void stable_update()
    {
        scenes->stable_update(get());
        scenes->cleanup(get());
    }

    // template<typename Engine>
    // static void stable_update_t(Engine& eng)
    // {
    //     eng.scenes->stable_update(eng);
    //     eng.scenes->cleanup(eng);
    // }

    // template<typename Engine>
    // static void frame_update_t(Engine& eng)
    // {
    //     eng.scenes->frame_update(eng);
    // }

    virtual void render(double between)
    {
        renderer->clear();

        renderer->prepare3D();

        draw_objects(between);

        renderer->prepare2D();

        draw_ui(between);

        window->swap_buffers();
    }

public:
    ptr<scene_manager<GameObject>> scenes = mk_ptr<scene_manager<GameObject>>();
    ptr<state> global;

    virtual ~engine_base() = default;

    engine_base(ptr<os::window_base> window, ptr<gx::renderer_base> renderer,
                ptr<state> global = nullptr)
        : window(std::move(window)),
          renderer(std::move(renderer)),
          global(std::move(global)) {}

    void play()
    {
        Timer timer;
        // DO NOT MAKE THIS VARIABLE UNSIGNED!
        int accum = 0;
        unsigned delta = global->physics_delta_us;

        init();

        while (!global->quit && !window->should_close())
        {
            frame_update();

            while (accum >= decltype(accum)(delta))
            {
                update_controls();
                stable_update();
                accum -= delta;
            }

            auto frame = timer.reset_duration_us();
            global->frame_time_us = frame;
            accum += frame;

            render(frame / static_cast<double>(delta));
        }
    }
};


} // namespace frog
