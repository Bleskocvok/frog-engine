
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
    auto engine = frog::ptr<frog::engine2d>{};

    auto path = frog::fs::path{ argc > 0 ? argv[0] : "./unknown" };

    auto global = frog::mk_ptr<frog::state>();

    auto set = frog::settings{};
    set.clear_color = { 0.5, 0.6, 0.3 };
    set.window_name = "gaaaaaame2d";
    set.vsync = true;
    // TODO:
    set.asset_path = "";
    set.save_path = "";

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
}


void add_objects(frog::engine2d& eng)
{

}
