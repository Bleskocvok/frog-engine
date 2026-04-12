#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "mixer_init.hpp"

#include <stdexcept>
#include <string>

namespace frog::lib2d {

class Channel;
class Audio;

class Sound
{
    friend Channel;
    friend Audio;

    detail::Chunk chunk;

    Sound(const char* filename)
            : chunk(Mix_LoadWAV(filename))
    {
        if (!chunk)
            throw std::runtime_error(std::string("Sound: ") + filename + ": " + Mix_GetError());
    }

public:
    unsigned length() const
    {
        int frequency = 22050;
        Uint16 format = MIX_DEFAULT_FORMAT;
        int channels = 2;
        Mix_QuerySpec(&frequency, &format, &channels);
        return (((chunk->alen / ((format & 0xff) / 8)) / channels) * 1000) / frequency;
    }
};

} // namespace frog::lib2d

#endif
