#pragma once

#include <filesystem>   // path
#include <string_view>
#include <string>


namespace frog
{


namespace fs = std::filesystem;


struct settings
{
    struct color
    {
        float r = 0,
              g = 0,
              b = 0;
    }
    clear_color;

    bool vsync = true;

    int width = 1280,
        height = 720;

    int gl_major = 4,
        gl_minor = 5;

    std::string window_name;


    void assign_path(const fs::path& executable)
    {
        auto parent = [](const auto& p)
        {
            return fs::canonical(p.parent_path());
        };

        auto folder = parent(parent(executable));

        // TODO this is kind of bad
        frog::fs::current_path(folder);
    }
};


} // namespace frog
