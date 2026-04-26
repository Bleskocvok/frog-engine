#pragma once

#include <cstdint>

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

namespace ios {

bool is_ipad();

const char* save_path(const char* org, const char* app);

const char* asset_path();

} // namespace ios


namespace android {

const char* save_path();

} // namespace android


} // namespace frog
