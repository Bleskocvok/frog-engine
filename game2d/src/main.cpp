
#include "core/engine2d.hpp"

#include "utils/load_file.hpp"
#include "utils/ptr.hpp"
#include "utils/debug.hpp"

#include <utility>      // move
#include <iostream>     // cout
#include <vector>


using namespace frog;


void init_game(frog::engine2d& eng);
void add_objects(frog::engine2d& eng);


int main(int argc, char** argv)
{
    auto engine = frog::ptr<frog::engine2d>{ nullptr };

    auto path = frog::fs::path{ argc > 0 ? argv[0] : "./unknown" };

    // TODO:
    auto asset_path = create_prog_relative_path(argv[0], "assets");
    std::string save_path = "";

    auto global = frog::mk_ptr<frog::state>(asset_path, save_path);

    auto set = frog::settings{};
    set.clear_color = { 0.5, 0.6, 0.3 };
    set.window_name = "gaaaaaame2d";
    set.vsync = true;

    //
    // Engine creation
    //
    try
    {
        engine = frog::mk_ptr<frog::engine2d>(std::move(set), std::move(global));
    }
    catch (std::exception& ex)
    {
        std::cerr << "FATAL ERROR: engine creation failed: "
                  << ex.what()
                  << std::endl;
        return 2;
    }

    //
    // Engine initialization and asset loading
    //
    try
    {
        init_game(*engine);
        add_objects(*engine);
    }
    catch (std::exception& ex)
    {
        std::cerr << "ERROR: engine failed during loading: "
                  << ex.what()
                  << std::endl;
        return 2;
    }

    //
    // Engine running
    //
    try
    {
        engine->play();
    }
    catch (std::exception& ex)
    {
        std::cerr << "ERROR: engine crashed: " << ex.what() << std::endl;
        return 2;
    }
    return 0;
}


void init_game(frog::engine2d& eng)
{
    using namespace frog;

    eng.add_texture("heart", "heart.png");
}


struct ballsack : frog::script2d
{
    void init(frog::game_object2d&, frog::engine2d& engine) override
    {
        using namespace frog;

        engine.win_raw->clear_color(124, 0, 123, 255);
    }

    void stable_update(frog::game_object2d& obj, frog::engine2d& engine) override
    {
        if (engine.input->k_at(SDL_SCANCODE_ESCAPE).released)
            engine.global->quit = true;

        // if (engine.input->has_resized())
        //     LOG(engine.input->has_resized()->first,
        //         engine.input->has_resized()->second);

        if (engine.input->k_at(SDL_SCANCODE_D).down) obj.model().rect.pos.x() += 0.01;
        if (engine.input->k_at(SDL_SCANCODE_A).down) obj.model().rect.pos.x() -= 0.01;
        if (engine.input->k_at(SDL_SCANCODE_S).down) obj.model().rect.pos.y() += 0.01;
        if (engine.input->k_at(SDL_SCANCODE_W).down) obj.model().rect.pos.y() -= 0.01;
    }
};


void add_objects(frog::engine2d& eng)
{
    using namespace frog;

    eng.scenes->add("main", mk_ptr<scene<game_object2d>>());

    auto gobj = mk_ptr<game_object2d>();
    gobj->add_script(mk_ptr<ballsack>());
    gobj->model().image_tag = "heart";

    eng.scenes->current().add(std::move(gobj));
}
