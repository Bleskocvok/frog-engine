#pragma once

namespace frog::target {

constexpr bool is_apple()
{
#ifdef __APPLE__
    return true;
#endif
    return false;
}

constexpr bool is_android()
{
#ifdef __ANDROID__
    return true;
#endif
    return false;
}

} // frog::target
