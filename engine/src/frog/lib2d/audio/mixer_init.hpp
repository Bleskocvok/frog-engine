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
    InitSound(std::uint32_t flags);

    ~InitSound();

    InitSound(const InitSound&) = delete;
    InitSound& operator=(const InitSound&) = delete;

    InitSound(InitSound&&) = default;
    InitSound& operator=(InitSound&&) = default;
};

namespace detail
{

struct SoundDeleter
{
    void operator()(Mix_Chunk* obj);
    void operator()(Mix_Music* obj);
};

using Chunk = std::unique_ptr<Mix_Chunk, SoundDeleter>;
using Music = std::unique_ptr<Mix_Music, SoundDeleter>;

} // namespace detail

} // namespace frog::lib2d

#endif
