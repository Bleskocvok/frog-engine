#pragma once

#include <cstdint>
#include <string>

namespace frog::mobile {

struct Orientation
{
    enum : std::uint32_t {
        Portrait            = 0x1,
        PortraitUpsideDown  = 0x2,
        LandscapeLeft       = 0x4,
        LandscapeRight      = 0x8
    };
};

void hint_orientations(std::uint32_t values);

const char* pref_path(const char* org, const char* app);

namespace ios {

bool is_ipad();

const char* asset_path();

} // namespace ios


namespace android {

const char* asset_path();

// Would need link against some lib.
// void log(const std::string& app, const std::string& str);

} // namespace android


} // namespace frog::mobile
