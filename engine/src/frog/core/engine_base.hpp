#pragma once

#include "frog/graphics/renderer_base.hpp"

#include "frog/os/window_base.hpp"
#include "frog/utils/ptr.hpp"

#include "scene_manager.hpp"
#include "state.hpp"

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
    virtual void reset_controls() = 0;
    virtual void frame_update() { scenes->frame_update(get()); }

    virtual void stable_update()
    {
        scenes->stable_update(get());
        scenes->end_update(get());
        scenes->cleanup(get());
    }

    void end_frame_update()
    {
        scenes->end_frame_update(get());
    }

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

    geo::ivec2 window_size() const { return { window->width(), window->height() }; }

    void play()
    {
        Timer timer;
        // DO NOT MAKE THIS VARIABLE UNSIGNED!
        int accum = 0;
        unsigned delta = global->physics_delta_us;

        init();

        while (!global->quit && !window->should_close())
        {
            update_controls();
            frame_update();

            // int i = 0;
            while (accum >= decltype(accum)(delta))
            {
                // Instead of updating controls directly before every
                // ‹stable_update›, let's do it in a way that ‹frame_update› has
                // access to the newest events, while ‹stable_update› does not
                // miss any.
                stable_update();
                reset_controls();
                update_controls();
                accum -= delta;

                // TODO: Consider the situation when stable_update takes longer
                // than delta. That way, accum keeps increasing, leading to a
                // drastic FPS drop. How solve?
                // if (++i > 2)
                // {
                //     accum = 0;
                //     break;
                // }
            }

            end_frame_update();

            auto frame = timer.reset_duration_us();
            global->frame_time_us = frame;
            accum += frame;

            render(frame / static_cast<double>(delta));
        }
    }
};


} // namespace frog
