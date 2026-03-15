#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "sound.hpp"
#include "channel.hpp"

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

namespace frog::lib2d {

class Audio
{
    struct Entry
    {
        std::unique_ptr<Channel> channel_ptr;
        bool assigned = true;

        Entry(Channel channel)
                : channel_ptr(std::make_unique<Channel>(std::move(channel)))
        { }
    };

    unsigned allocated_channels = 8;

    std::vector<Entry> channels;

public:
    Audio(int frequency = 22050, int channels = 2, int chunk_size = 1024)
    {
        if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunk_size) != 0)
            throw std::runtime_error(std::string("Create Audio: ") + Mix_GetError());
    }

    ~Audio()
    {
        Mix_CloseAudio();
    }

    Sound make_sound(const char* filename)
    {
        return Sound(filename);
    }

    Channel& add_channel()
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

    void remove_channel(Channel& channel)
    {
        channels.at(channel.get_index()).assigned = false;
    }
};

} // namespace frog::lib2d

#endif
