#pragma once

namespace frog::target {

constexpr bool is_apple_ios()
{
#ifdef TARGET_OS_IOS
    return true;
#endif
    return false;
}

constexpr bool is_apple_mac()
{
    if (is_apple_ios())
        return false;

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
