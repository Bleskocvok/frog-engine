#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "sound.hpp"

namespace frog::lib2d {

class Audio;

class Channel
{
    friend Audio;

    int index;

    Channel(int index)
            : index(index)
    { }

public:
    static constexpr int MaxVolume = MIX_MAX_VOLUME;

    void play(Sound& sound)
    {
        Mix_PlayChannel(index, sound.chunk.get(), 0);
    }

    void fade_in(Sound& sound, int ms)
    {
        Mix_FadeInChannel(index, sound.chunk.get(), 0, ms);
    }

    bool is_playing() const
    {
        return Mix_Playing(index);
    }

    void halt()
    {
        Mix_HaltChannel(index);
    }

    void fade_out(int ms)
    {
        Mix_FadeOutChannel(index, ms);
    }

    void pause()
    {
        Mix_Pause(index);
    }

    void unpause()
    {
        Mix_Resume(index);
    }

    void set_volume(int volume)
    {
        Mix_Volume(index, volume);
    }

    int get_volume() const
    {
        return Mix_Volume(index, -1);
    }

    int get_index() const
    {
        return index;
    }
};

} // namespace frog::lib2d

#endif
