
#include "frog/core/engine2d.hpp"

#include "frog/utils/load_file.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/utils/debug.hpp"

#include <utility>      // move
#include <iostream>     // cout
#include <vector>


using namespace frog;


void init_game(frog::engine2d& eng);
void add_objects(frog::engine2d& eng);


int main(int argc, char** argv)
{
    auto path = frog::fs::path{ argc > 0 ? argv[0] : "./unknown" };

    // TODO: solve save_path
    auto asset_path = create_prog_relative_path(argv[0], "assets");
    std::string save_path = "";

    auto global = frog::mk_ptr<frog::state>(asset_path, save_path);

    auto set = frog::settings{};
    set.clear_color = { 0.5, 0.6, 0.3 };
    set.window_name = "gaaaaaame2d";
    set.vsync = true;

    return run_engine2d(std::move(set), std::move(global),
                        [](frog::engine2d& e){ init_game(e); },
                        [](frog::engine2d& e){ add_objects(e); });
}


void init_game(frog::engine2d& eng)
{
    using namespace frog;

    eng.add_texture("font", "font.png");

    eng.add_texture("box", "box.png");
    eng.add_texture("circle", "circle.png");
}





#include "frog/geometry/physics.hpp"

#include <unordered_map>

struct ballsack : frog::script2d
{
    void init(frog::game_object2d& obj, frog::engine2d& engine) override
    {
        using namespace frog;

        // engine.win_raw->clear_color(124, 0, 123, 255);

        resize(engine.win_raw->w(), engine.win_raw->h(), engine);

        physics.settings_.universum = { 0, 0, 1, 1 };
        physics.settings_.iterations = 5;

        // auto* label_fps = obj.add_element(mk_ptr<gx::ui_element>());
        // label_fps->label = gx::text{ "ABCDEFGHIJKL" };
        // label_fps->pos = geo::vec2{ 0 };
        // label_fps->size = geo::vec2{ 0.3, 0.1 };
        // label_fps->sprite = "circle";
        // label_fps->tex_pos = { 0, 0, };
        // label_fps->tex_size = { 0.5, 0.5 };
    }

    void resize(int w, int h, frog::engine2d& eng)
    {
        eng.camera.size = { w / float(h), 1 };
    }

    geo::soft_physics2d physics;
    std::unordered_map<decltype(physics)::idx_t, game_object2d*> babies;

    void stable_update(frog::game_object2d& obj, frog::engine2d& engine) override
    {
        // LOGX(engine.global->fps());
        if (engine.input->k_at(SDL_SCANCODE_ESCAPE).released)
            engine.global->quit = true;

        if (engine.input->k_at(SDL_SCANCODE_SPACE).released)
            throw std::runtime_error("Spacebar pressed");

        if (engine.input->has_resized())
        {
            auto [w, h] = engine.input->has_resized().value();
            resize(w, h, engine);
        }

        auto add_baby = [&](auto pos) -> decltype(physics)::idx_t
        {
            // float size = 0.1;
            float size = 0.05;
            // float size = 0.025;
            auto gobj = mk_ptr<game_object2d>();
            gobj->model().image_tag = "circle";
            gobj->model().rect.size = { size, size };
            gobj->model().rect.pos = pos;

            auto* ptr = engine.scenes->current().add(std::move(gobj));
            auto idx = physics.add_point(decltype(physics)::point
            {
                .pos = pos,
                .prev = pos,
                .radius = size / 2,
                .inv_weight = 1,
            });
            babies.emplace(idx, ptr);
            return idx;
        };

        if (engine.input->mouse().but_l.pressed
         || engine.input->k_at(SDL_SCANCODE_F).down)
        {
            geo::vec2 pos = engine.camera_coords(engine.input->mouse());
            auto a = add_baby(pos);
            auto b = add_baby(pos + geo::vec2(0, 0.1));
            auto c = add_baby(pos + geo::vec2(0.1, 0.1));
            // auto d = add_baby(pos + geo::vec2(-0.1, 0.1));

            physics.add_joint_between(a, b);
            physics.add_joint_between(b, c);
            // physics.add_joint_between(b, d);

            // physics.add_angle(decltype(physics)::angle
            // {
            //     .a = a,
            //     .b = b,
            //     .c = c,
            //     // .angle = geo::Pi / 2,
            //     .angle = geo::Pi - 0.2,
            // });
        }

        for (auto&[idx, pt] : physics.points())
        {
            babies.at(idx)->model().rect.pos = pt.pos;
        }
        physics.update();
    }
};


#include "frog/scripts/fps_script.hpp"

void add_objects(frog::engine2d& eng)
{
    using namespace frog;

    eng.scenes->add("main", mk_ptr<scene<game_object2d>>());

    auto gobj = mk_ptr<game_object2d>();
    gobj->add_script(mk_ptr<ballsack>());
    gobj->add_script(mk_ptr<fps_script>());
    gobj->model().image_tag = "box";
    gobj->model().color = { 0.3, 0.3, 0.3, 1 };

    eng.scenes->current().add(std::move(gobj));
}
