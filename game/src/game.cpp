
#include "frog/core/engine3d.hpp"
#include "frog/core/object_builder.hpp"
#include "frog/gx3d/shape.hpp"

#include "frog/utils/fs.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/utils/debug.hpp"

#include "constants.hpp"
#include "paddle_script.hpp"
#include "wall_script.hpp"
#include "ball_script.hpp"
#include "bounce_script.hpp"
#include "fps_script.hpp"
#include "game_script.hpp"
#include "player_script.hpp"
#include "camera_script.hpp"
#include "game_over_script.hpp"
#include "quit_script.hpp"

#include <utility>      // move
#include <iostream>     // cout
#include <vector>


using namespace frog;


void init_game(frog::engine& eng);
void add_objects(frog::engine& eng);


int main(int argc, char** argv)
{
    auto engine = frog::ptr<frog::engine>{};

    auto path = frog::fs::path{ argc > 0 ? argv[0] : "./unknown" };

    // TODO: solve save_path
    auto asset_path = create_prog_relative_path(argv[0], "");
    std::string save_path = "";

    auto global = frog::mk_ptr<frog::state>(asset_path, save_path);

    auto set = frog::settings{};
         set.clear_color = { 0.5, 0.6, 0.3 };
         set.window_name = "gaaaaaame";
         set.vsync = true;

    //
    // Engine creation
    //
    try
    {
        engine = frog::mk_ptr<frog::engine>(std::move(set), std::move(global));
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


void init_game(frog::engine& eng)
{
    using namespace frog;
    using std::pair, std::string;
    using entry = pair<string, gl::mesh>;

    auto meshes = std::array
    {
        entry{ "sphere", gl::shape::sphere(10, 20, Ball::Radius) },
        entry{ "ground", gl::shape::polygon_2D(40, 1) },
        entry{ "paddle", gl::shape::paddle(
                            Sides,
                            Paddle::Radius,
                            Paddle::Thickness,
                            Paddle::Height,
                            Paddle::AngularSize) },
        entry{ "wall", gl::shape::paddle(
                            Sides,
                            Wall::Radius,
                            Wall::Thickness,
                            Wall::Height,
                            Wall::AngularSize) },
        entry{ "powerup", gl::shape::sphere(10, 20, PowerupSize) },
    };

    auto textures = std::vector<pair<string, string>>
    {
        { "ground", "game_assets/ground.png" },
        { "heart", "game_assets/heart.png" },
    };

    auto scenes = std::array
    {
        "main",
        "game_over",
    };

    for (auto& [tag, shape] : meshes)
    {
        eng.meshes.add(std::move(tag), std::move(shape));
    }

    for (auto& [tag, tex] : textures)
    {
        eng.add_texture(tag, tex);
    }

    for (auto& name : scenes)
    {
        eng.scenes->add(std::move(name), mk_ptr<scene<game_object>>());
    }
}


void add_objects(frog::engine& eng)
{
    object_builder builder;

    //
    // Add ball
    //
    builder.reset();
    builder.add_mesh("sphere");
    builder.set_tag("ball");
    builder.emplace_script<ball_script>();
    builder.set_material(gl::material
        {
            geo::vec4{ 0.05, 0.05, 0.05, 1 },
            geo::vec4{ 0.6, 0.6, 0.6, 1 },
            geo::vec4{ 1, 1, 1, 4 },
        });
    builder.build_in(eng);

    //
    // Add paddles
    //
    gl::material mat =
    {
        geo::vec4(0.1, 0.1, 0.2, 1),
        geo::vec4(0.2, 0.4, 0.9, 1),
        geo::vec4(0.95, 0.95, 0.95, 200),
    };
    for (int i = 0; i < Paddle::Count; i++)
    {
        float angle = 360 / Paddle::Count * i + 90 - Paddle::AngularSize / 2;
        builder.reset();
        builder.add_mesh("paddle");
        builder.set_tag("paddle");
        builder.set_material(mat);
        builder.emplace_script<paddle_script>(i == 0);
        builder.emplace_script<bounce_script>(angle, 0, Paddle{});
        builder.build_in(eng);
    }

    //
    // Add ground
    //
    builder.add_mesh("ground");
    builder.add_texture("ground");
    builder.set_scale({ GroundSize });
    builder.emplace_script<player_script>();
    // (walls are initalized in game_script)
    builder.emplace_script<game_script>();
    builder.build_in(eng);

    //
    // Add other
    //

    // fps counter
    builder.reset();
    builder.emplace_script<fps_script>();
    builder.build_in(eng);

    // escape to quit game
    builder.reset();
    builder.emplace_script<quit_script>(GLFW_KEY_ESCAPE);
    builder.build_in(eng);

    builder.reset();
    builder.emplace_script<camera_script>();
    builder.build_in(eng);

    // game over screen
    builder.reset();
    builder.emplace_script<game_over_script>();
    eng.scenes->at("game_over").add(builder.build());

    // escape to quit game
    builder.reset();
    builder.emplace_script<quit_script>(GLFW_KEY_ESCAPE);
    eng.scenes->at("game_over").add(builder.build());
}

