#include "mobile.hpp"

#ifndef NOT_FROG_BUILD_2D

#include <stdexcept>    // runtime_error
#include <string>

#if __APPLE__
    #if TARGET_OS_IPHONE
        #define __IPHONEOS__
    #endif
#endif


#if defined(__ANDROID__) || defined(__IPHONEOS__)
    #include <SDL2/SDL.h>
#else
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

void hint_orientations(std::uint32_t values)
{
    std::string str;

    auto put = [&](const char* s)
    {
        if (not str.empty())
            str += ' ';
        str += s;
    };

    if (values & Orientation::LandscapeLeft)      put("LandscapeLeft");
    if (values & Orientation::LandscapeRight)     put("LandscapeRight");
    if (values & Orientation::Portrait)           put("Portrait");
    if (values & Orientation::PortraitUpsideDown) put("PortraitUpsideDown");

    SDL_SetHint(SDL_HINT_ORIENTATIONS, str.c_str());
}

bool ios::is_ipad()
{
#ifndef __IPHONEOS__
    throw Error("Not building for iOS");
#else
    return is_ipad_impl();
#endif
}


const char* pref_path(const char* org, const char* app)
{
    // TODO: SDL_free() this
    const char* pref = SDL_GetPrefPath(org, app);
    return pref;
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

const char* android::asset_path()
{
#ifndef __ANDROID__
    throw Error("Not building for Android");
#else
    const char* path = SDL_AndroidGetInternalStoragePath();
    // using namespace std::string_literals;
    // if (path == nullptr)
    //     throw Error("Android save path: "s + SDL_GetError());
    return path;
#endif
}

} // namespace frog::mobile


// #ifdef __ANDROID__

// #include <android/log.h>

// void frog::mobile::android::log(const std::string& app, const std::string& str)
// {
//     __android_log_print(ANDROID_LOG_INFO, app.c_str(), "%s", str.c_str());
// }
// #endif

#endif
