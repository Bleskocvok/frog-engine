#ifndef NOT_FROG_BUILD_2D

#include "audio.hpp"

#include "sound.hpp"
#include "channel.hpp"


namespace frog::lib2d {

Audio::Audio(int frequency, int channels, int chunk_size)
{
    if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunk_size) != 0)
        throw std::runtime_error(std::string("Create Audio: ") + Mix_GetError());
}

Audio::~Audio()
{
    Mix_CloseAudio();
}

Sound Audio::make_sound(const char* filename)
{
    return Sound(filename);
}

Channel& Audio::add_channel()
{
    for (Entry& entry : channels)
    {
        if (!entry.assigned)
        {
            entry.assigned = true;
            return *entry.channel_ptr;
        }
    }
    if (channels.size() + 1 > allocated_channels)
    {
        Mix_AllocateChannels(allocated_channels * 2);
        allocated_channels *= 2;
    }
    channels.emplace_back(Channel(channels.size()));
    return *channels.back().channel_ptr;
}

void Audio::remove_channel(Channel& channel)
{
    channels.at(channel.get_index()).assigned = false;
}

} // namespace frog::lib2d

#endif
