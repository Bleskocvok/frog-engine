#ifndef NOT_FROG_BUILD_2D

#include "audio.hpp"

#include "sound.hpp"
#include "channel.hpp"

#include "frog/debug.hpp"
#include "frog/utils/string_builder.hpp"

namespace frog::lib2d {

Audio::Audio(int frequency, int channels, int chunk_size)
{
    if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunk_size) != 0)
    {
        // TODO: Allow for no audio and don't crash.
        // throw std::runtime_error(std::string("Create Audio: ") + Mix_GetError());

        auto str = make_string("Create Audio: ", Mix_GetError());
        LOG(str);
        error.emplace(str);
    }
}

Audio::~Audio()
{
    Mix_CloseAudio();
}

Sound Audio::make_sound(const char* filename)
{
    return Sound(filename, error.has_value());
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
