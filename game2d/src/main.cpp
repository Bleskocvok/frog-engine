
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





#include "frog/utils/ptr.hpp"

#include "frog/core/engine2d.hpp"

#include "frog/geometry/physics.hpp"
#include "frog/geometry/basic.hpp"
#include "frog/geometry/transform.hpp"

#include <unordered_map>
#include <random>

struct ball_t
{
    frog::game_object2d* obj = nullptr;
    frog::geo::soft_physics2d::idx_t idx;
};

struct balls : frog::script2d
{
    frog::geo::soft_physics2d physics = { decltype(physics)::limits{ 20000, 20000, 200 } };
    std::vector<ball_t> babies;

    frog::gx::ui_element* ui = nullptr;

    void init(frog::game_object2d& obj, frog::engine2d&) override
    {
        using namespace frog;

        physics.settings().universum = { 0, 0, 1, 1 };
        physics.settings().iterations = 5;

        static constexpr float label_height = 0.03;
        ui = obj.add_element(mk_ptr<gx::ui_element>());
        ui->label = { "count:", 1 };
        ui->pos() = geo::vec2(-0.5, -0.5 + label_height * 1.5);
        ui->size() = { label_height };
    }

    void stable_update(frog::game_object2d& obj, frog::engine2d& engine) override
    {
        using namespace frog;

        ui->label->str = make_string("count: ", babies.size());

        if (engine.input->k_at(SDL_SCANCODE_F).down)
        {
            if (!physics.limit_reached())
                add_screw(engine);

            if (engine.input->k_at(SDL_SCANCODE_LSHIFT).down)
                for (int i = 0; i < 10; ++i)
                    if (!physics.limit_reached())
                        add_screw(engine);
        }

        if (engine.input->mouse().but_l.pressed || engine.input->mouse().but_l.down)
            for (int i = 0; i < 1 + 10 * engine.input->k_at(SDL_SCANCODE_LSHIFT).down; ++i)
                add_screw(engine, 0.5);

        if (engine.input->mouse().but_r.pressed || engine.input->mouse().but_r.down)
            for (int i = 0; i < 1 + 10 * engine.input->k_at(SDL_SCANCODE_LSHIFT).down; ++i)
                add_screw(engine, 3);

        // static auto gen = std::mt19937{ 0 };
        // static auto dist = std::uniform_real_distribution<float>(0.5, 3);

        if (engine.input->k_at(SDL_SCANCODE_B).down)
        {
            if (!babies.empty())
                delete_screw(rand() % babies.size());

            if (engine.input->k_at(SDL_SCANCODE_LSHIFT).down)
                for (int i = 0; i < 9; ++i)
                    if (!babies.empty())
                        delete_screw(rand() % babies.size());
        }

        update_screws();
    }

    void delete_screw(int i)
    {
        babies.at(i).obj->destroy();
        physics.remove_point(babies.at(i).idx);
        babies.erase(babies.begin() + i);
    }

    void add_screw(frog::engine2d& engine, float size_mult = 1)
    {
        auto add_ball = [&](float size, auto pos)
            -> std::pair<frog::game_object2d*, decltype(physics)::idx_t>
        {
            auto gobj = frog::mk_ptr<frog::game_object2d>();
            gobj->model().image_tag = "circle";
            gobj->model().rect.size = { size, size };
            gobj->model().rect.pos = pos;
            gobj->model().layer = 1;
            auto* ptr = engine.scenes->current().add(std::move(gobj));

            auto idx = physics.add_point(decltype(physics)::point
            {
                .pos = pos,
                .prev = pos,
                .radius = size / 2,
                .inv_weight = 1 / size,
            });
            return { ptr, idx };
        };

        using namespace frog::geo;
        using frog::mk_ptr, frog::game_object2d, frog::make_string;

        auto ball = ball_t{};

        auto circ = circle{ vec2{ 0, 0.055 }, 0.04 };
        circ.radius *= size_mult;

        frog::geo::vec2 pos = engine.camera_coords(engine.input->mouse());
        auto[obj, idx] = add_ball(circ.radius, pos);

        ball.obj = obj;
        ball.idx = idx;

        babies.push_back(std::move(ball));
    }

    void update_screws()
    {
        physics.update();

        for (auto& ball : babies)
        {
            ball.obj->model().rect.pos = physics.point_at(ball.idx).pos;
        }
    }
};


struct controls : frog::script2d
{
    void init(frog::game_object2d& obj, frog::engine2d& engine) override
    {
        using namespace frog;
        engine.win_raw->clear_color(0, 0, 0, 255);
        resize(engine.win_raw->w(), engine.win_raw->h(), engine);
    }

    void resize(int w, int h, frog::engine2d& eng)
    {
        eng.camera.size = { w / float(h), 1 };
    }

    void stable_update(frog::game_object2d& obj, frog::engine2d& engine) override
    {
        if (engine.input->k_at(SDL_SCANCODE_ESCAPE).released)
            engine.global->quit = true;

        if (engine.input->has_resized())
        {
            auto [w, h] = engine.input->has_resized().value();
            resize(w, h, engine);
        }
    }
};

#include "frog/scripts/fps_script.hpp"

void add_objects(frog::engine2d& eng)
{
    using namespace frog;

    eng.scenes->add("main", mk_ptr<scene<game_object2d>>());

    auto gobj = mk_ptr<game_object2d>();
    gobj->add_script(mk_ptr<balls>());
    gobj->add_script(mk_ptr<fps_script>());
    gobj->add_script(mk_ptr<controls>());
    gobj->model().image_tag = "box";
    gobj->model().color = { int(0.3 * 255), int(0.3 * 255), int(0.3 * 255), 255 };

    eng.scenes->current().add(std::move(gobj));
}
