#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "frog/graphics/assets.hpp"
#include "frog/lib2d/audio/audio.hpp"
#include "frog/lib2d/audio/sound.hpp"
#include "frog/lib2d/audio/channel.hpp"
#include "frog/utils/ptr.hpp"
#include "frog/utils/assert.hpp"

#include <string>
#include <utility>
#include <vector>

namespace frog {

class Audio;

class Channels
{
protected:
    lib2d::Audio* audio = nullptr;
    std::vector<lib2d::Channel*> channels_;

public:
    Channels(lib2d::Audio* audio)
        : audio(audio)
    { }

    const auto& data() const { return channels_; }

    lib2d::Channel& add()
    {
        frog_assert(audio);
        auto& chn = audio->add_channel();
        channels_.push_back(&chn);
        return chn;
    }

    void remove(lib2d::Channel& chn)
    {
        frog_assert(audio);
        audio->remove_channel(chn);
        std::erase(channels_, &chn);
    }

    void clear()
    {
        frog_assert(audio);
        for (auto* chn : channels_)
            if (chn)
                audio->remove_channel(*chn);

        channels_.clear();
    }

    void resize(int n)
    {
        clear();

        while (n-- > 0)
        {
            add();
        }
    }
};

// struct Sound
// {
//     virtual void play(Chn);
// };

class SpareChannels : public Channels
{
    unsigned next_ = 0;

public:
    SpareChannels(lib2d::Audio* audio)
        : Channels(audio)
    { }

    lib2d::Channel* next()
    {
        if (data().empty())
            return nullptr;

        auto* ret = channels_[next_];
        next_ = (next_ + 1) % data().size();
        return ret;
    }

    void play(lib2d::Sound& sound)
    {
        frog::assert_ptr( next() )->play(sound);
    }
};

class Group
{
    Audio* audio = nullptr;
    std::vector<std::string> sounds;

    SpareChannels spare_channels;

public:
    Group(Audio& audio_);

    bool add_sound(const std::string& path);

    bool remove_sound(const std::string& tag);

    std::string get();

    void quick_play();

    void play(lib2d::Channel& chn);

    void set_spare_channels(int count);
};

class Audio
{
    lib2d::InitSound init_sound = lib2d::InitSound( lib2d::Format::OGG );

    std::string path_prefix;

    lib2d::Audio audio;

    gx::assets<lib2d::Sound> sounds;

    gx::assets<ptr<Group>> groups;

    static constexpr int FREQUENCY = 22050;
    static constexpr int CHANNELS = 2;
    static constexpr int CHUNK_SIZE = 1024;

    static constexpr int SPARE_CHANNELS = 8;

    void quick_play(lib2d::Sound& sound);

    friend Group;

    SpareChannels spare_channels;

public:
    Channels channels;

    Audio(std::string path_prefix)
        : path_prefix(std::move(path_prefix))
        , audio(FREQUENCY, CHANNELS, CHUNK_SIZE)
        , spare_channels(&audio)
        , channels(&audio)
    {
        set_spare_channels(SPARE_CHANNELS);
    }

    Group* group_at(const std::string& tag)
    {
        auto* ptr = groups.find(tag);
        if (not ptr)
            return nullptr;
        return ptr->get();
    }

    Group* add_group(const std::string& tag)
    {
        groups.add(tag, mk_ptr<Group>(*this));
        return groups.at(tag).get();
    }

    bool add_sound(const std::string& tag, const std::string& path);

    bool remove_sound(const std::string& tag);

    void quick_play(const std::string& tag);

    void play(const std::string& tag);

    void play(const std::string& tag, lib2d::Channel& chn);

    void set_spare_channels(int count);
};

} // namespace frog

#endif
