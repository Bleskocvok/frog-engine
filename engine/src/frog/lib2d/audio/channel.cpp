#ifndef NOT_FROG_BUILD_2D

#include "channel.hpp"

namespace frog::lib2d {

void Channel::play(Sound& sound)
{
    Mix_PlayChannel(index, sound.chunk.get(), 0);
}

void Channel::fade_in(Sound& sound, int ms)
{
    Mix_FadeInChannel(index, sound.chunk.get(), 0, ms);
}

bool Channel::is_playing() const
{
    return Mix_Playing(index);
}

void Channel::halt()
{
    Mix_HaltChannel(index);
}

void Channel::fade_out(int ms)
{
    Mix_FadeOutChannel(index, ms);
}

void Channel::pause()
{
    Mix_Pause(index);
}

void Channel::unpause()
{
    Mix_Resume(index);
}

void Channel::set_volume(int volume)
{
    Mix_Volume(index, volume);
}

int Channel::get_volume() const
{
    return Mix_Volume(index, -1);
}

} // namespace frog::lib2d

#endif
