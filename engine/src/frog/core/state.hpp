#pragma once

#include <string>

namespace frog
{


struct state
{
    std::string m_asset_path;
    std::string m_save_path;
public:
    state(std::string m_asset_path, std::string m_save_path)
        : m_asset_path(m_asset_path), m_save_path(m_save_path) {}

    unsigned physics_delta_us = 20000;  // default value = 20 ms = 50 fps
    unsigned frame_time_us    = 20000;  // default value = 20 ms = 50 fps

    bool quit = false;

    // in seconds
    double stable_frame_time() const { return physics_delta_us / 1000000.0; }

    // in seconds
    double frame_time() const { return frame_time_us / 1000000.0; }

    double fps() const { return 1.0 / frame_time(); }

    const auto& asset_path() const { return m_asset_path; }
    const auto& save_path() const { return m_save_path; }
};


} // namespace frog

