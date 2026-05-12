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
    Audio(int frequency = 22050, int channels = 2, int chunk_size = 1024);

    Audio(const Audio&) = delete;

    Audio& operator=(Audio&) = delete;

    ~Audio();

    Sound make_sound(const char* filename);

    Channel& add_channel();

    void remove_channel(Channel& channel);
};

} // namespace frog::lib2d

#endif
