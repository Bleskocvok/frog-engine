#pragma once

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


struct settings
{
    struct color
    {
        float r = 0,
              g = 0,
              b = 0;

    } clear_color;

    bool vsync = true;

    int width = 1280,
        height = 720;

    int gl_major = 4,
        gl_minor = 5;

    std::string window_name;

    std::string save_path;
    std::string asset_path;

    std::optional<std::string> window_icon;

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
