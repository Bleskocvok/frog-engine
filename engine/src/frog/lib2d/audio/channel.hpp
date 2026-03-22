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

    void play(Sound& sound);

    void fade_in(Sound& sound, int ms);

    bool is_playing() const;

    void halt();

    void fade_out(int ms);

    void pause();

    void unpause();

    void set_volume(int volume);

    int get_volume() const;

    int get_index() const
    {
        return index;
    }
};

} // namespace frog::lib2d

#endif
