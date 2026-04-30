#pragma once

namespace frog::target {

#ifdef __APPLE__
    #include <TargetConditionals.h>
#endif

constexpr bool is_apple_ios()
{
#if TARGET_OS_IPHONE
    return true;
#endif
    return false;
}

constexpr bool is_apple_mac()
{
    if (is_apple_ios())
        return false;

#ifdef TARGET_OS_OSX
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
