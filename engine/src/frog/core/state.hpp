#pragma once

#include <string>

namespace frog
{


struct state
{
    std::string m_asset_path;
    std::string m_save_path;

    static void path_normalize(std::string& path)
    {
        if (path.empty())
            path = ".";

        // // Android has trouble with double /
        // if (path.ends_with('/'))
        //     path.pop_back();
    }

public:
    state(std::string asset_path_, std::string save_path_)
        : m_asset_path(asset_path_), m_save_path(save_path_)
    {
        path_normalize(m_asset_path);
        path_normalize(m_save_path);
    }

    unsigned physics_delta_us = 20000;  // default value = 20 ms = 50 fps
    unsigned frame_time_us    = 20000;  // default value = 20 ms = 50 fps

    bool quit = false;

    unsigned max_consecutive_updates = 3;

    double between_ = 0;

    // in seconds
    double stable_frame_time() const { return physics_delta_us / 1000000.0; }

    // in seconds
    double frame_time() const { return frame_time_us / 1000000.0; }

    double fps() const { return 1.0 / frame_time(); }

    double between() const { return between_; }

    const auto& asset_path() const { return m_asset_path; }
    const auto& save_path() const { return m_save_path; }

    std::string in_save_path(const std::string& file) const
    {
        auto path = save_path();
        if (not path.ends_with('/'))
            path += '/';
        path += file;
        return path;
    }

    bool init_obj_immediately = true;
    bool init_obj_recursive = true;
};


} // namespace frog

