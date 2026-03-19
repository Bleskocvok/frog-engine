#pragma once

#ifndef NOT_FROG_BUILD_2D

#include "frog/graphics/assets.hpp"
#include "frog/lib2d/audio/audio.hpp"
#include "frog/lib2d/audio/sound.hpp"
#include "frog/utils/ptr.hpp"

#include <string>
#include <utility>
#include <vector>

namespace frog {

class Audio;

class Group
{
    Audio* audio = nullptr;
    std::vector<std::string> sounds;

public:
    Group(Audio& audio)
        : audio(&audio)
    { }

    bool add_sound(const std::string& tag, const std::string& path);

    bool remove_sound(const std::string& tag);

    void quick_play();

    void play(lib2d::Channel& chn);
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

public:
    Audio(std::string path_prefix)
        : path_prefix(std::move(path_prefix))
        , audio(FREQUENCY, CHANNELS, CHUNK_SIZE)
    { }

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

    void quick_play(lib2d::Sound& sound);
};

} // namespace frog

#endif
