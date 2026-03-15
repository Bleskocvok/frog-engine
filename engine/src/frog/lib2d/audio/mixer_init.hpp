#pragma once

#ifndef NOT_FROG_BUILD_2D

// #include "frog/lib2d/sdl_include.hpp"
// #include SDL_MIXER_HEADER
#include "frog/lib2d/include/mixer.hpp"

#include <stdexcept>
#include <memory>
#include <cstdint>

namespace frog::lib2d {

enum Format : std::uint32_t { OGG = 0x1, WAV = 0x2 };

class InitSound
{
public:
    InitSound(std::uint32_t flags)
    {
        int libflags = 0;

        if (flags & Format::OGG)
            libflags |= MIX_INIT_OGG;

        if ((Mix_Init(libflags) & libflags) != libflags)
            throw std::runtime_error(Mix_GetError());
    }

    ~InitSound()
    {
        Mix_Quit();
    }

    InitSound(const InitSound&) = delete;
    InitSound& operator=(const InitSound&) = delete;

    InitSound(InitSound&&) = default;
    InitSound& operator=(InitSound&&) = default;
};

namespace detail
{

struct SoundDeleter
{
    void operator()(Mix_Chunk* obj) { Mix_FreeChunk(obj); }
    void operator()(Mix_Music* obj) { Mix_FreeMusic(obj); }
};

using Chunk = std::unique_ptr<Mix_Chunk, SoundDeleter>;
using Music = std::unique_ptr<Mix_Music, SoundDeleter>;

} // namespace detail

} // namespace frog::lib2d

#endif
