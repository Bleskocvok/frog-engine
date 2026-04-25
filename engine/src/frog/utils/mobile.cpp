#include "mobile.hpp"

#include <stdexcept>    // runtime_error

#if __APPLE__
    #if TARGET_OS_IPHONE
        #define __IPHONEOS__
    #endif
#endif


#if defined(__ANDROID__) || defined(__IPHONEOS__)
#include <SDL2/SDL.h>
#endif

#ifdef __IPHONEOS__
    extern "C" int frog_is_ipad(void);

    namespace {

    bool is_ipad_impl()
    {
        return frog_is_ipad();
    }

    } // namespace
#else
    namespace {

    bool is_ipad_impl()
    {
        return false;
    }

    } // namespace
#endif

namespace frog::mobile {

using Error = std::runtime_error;

bool ios::is_ipad()
{
#ifndef __IPHONEOS__
    throw Error("Not building for iOS");
#else
    return is_ipad_impl();
#endif
}


const char* ios::save_path()
{
#ifndef __IPHONEOS__
    throw Error("Not building for iOS");
#else
    // TODO: SDL_free() this
    const char* pref = SDL_GetPrefPath(ZM_IOS_PREF_ORG, ZM_IOS_PREF_APP);
    return pref;
#endif
}

const char* ios::asset_path()
{
#ifndef __IPHONEOS__
    throw Error("Not building for iOS");
#else
    // TODO: SDL_free() this
    const char* base = SDL_GetBasePath();
    return base;
#endif
}

const char* android::save_path()
{
#ifdef __ANDROID__
    return SDL_AndroidGetInternalStoragePath();
#else
    throw Error("Not building for Android");
#endif
}

} // namespace frog
