#pragma once

#include "frog/lib2d/window.hpp"

#include <filesystem>   // path
#include <string>
#include <optional>


namespace frog
{


namespace fs = std::filesystem;


inline std::string create_prog_relative_path(const std::string& argv0,
    const std::string& relative_path)
{
    fs::path path = argv0;
    return path.parent_path() / relative_path;
}

enum class ScreenMode : int
{
    Windowed = 0, Borderless = 1, Fullscreen = 2, /* TODO:FullscreenBorderless = 3, */
};


struct settings
{
    struct color
    {
        float r = 0,
              g = 0,
              b = 0;

    } clear_color;

    int gl_major = 4,
        gl_minor = 5;

    std::string save_path;
    std::string asset_path;

    using WindowPosition = frog::lib2d::gx::WindowPosition;
    using winpos_undefined = frog::lib2d::gx::winpos_undefined;
    using winpos_centered = frog::lib2d::gx::winpos_centered;

    struct
    {
        bool vsync = true;

        int width = 1280,
            height = 720;

        ScreenMode mode = ScreenMode::Windowed;

        std::string title;

        bool maximized = false;
        WindowPosition pos_x = winpos_undefined{};
        WindowPosition pos_y = winpos_undefined{};

        bool allow_high_dpi = false;

        std::optional<std::string> icon;

    } window;

    void assign_path(const fs::path& executable)
    {
        auto parent = [](const auto& p)
        {
            return fs::canonical(p.parent_path());
        };

        // TODO make sure the correct directory is used
        auto folder = parent(executable);

        // TODO this is kind of bad
        frog::fs::current_path(folder);
    }
};


} // namespace frog
