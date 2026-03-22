#include "frog/lib2d/audio/sound.hpp"
#include "frog/utils/assert.hpp"
#include <string>
#ifndef NOT_FROG_BUILD_2D

#include "frog/core/audio/audio.hpp"
#include "frog/utils/todo.hpp"

#include <vector>
#include <random>

namespace frog {

Group::Group(Audio& audio_)
    : audio(&audio_)
    , spare_channels(&audio_.audio)
{ }

bool Group::add_sound(const std::string& path)
{
    // TODO: What if sound of tag exists but is different path?
    // Exception? (No, just have as alias to that: complicated implementation,
    // do later.)
    const auto& tag = path;
    sounds.push_back(tag);
    return audio->add_sound(tag, path);
}

bool Group::remove_sound(const std::string& tag)
{
    std::erase(sounds, tag);
    return audio->remove_sound(tag);
}

std::string Group::get()
{
    if (sounds.empty())
        return "";

    static std::random_device rd{};
    static thread_local std::mt19937 gen{ rd() };

    auto dist = std::uniform_int_distribution<size_t>(0, sounds.size() - 1);
    return sounds[ dist(gen) ];
}

void Group::quick_play()
{
    const auto& sound = get();
    if (spare_channels.data().empty())
        audio->quick_play(sound), LOG("audio");
    else
        audio->play(sound, *frog::assert_ptr( spare_channels.next() )), LOG("spare");
}

void Group::set_spare_channels(int count)
{
    spare_channels.resize(count);
}

void Group::play(lib2d::Channel& chn)
{
    audio->play(get(), chn);
}

bool Audio::add_sound(const std::string& tag, const std::string& path)
{
    auto thing = path_prefix + "/" + path;
    return sounds.add(tag, audio.make_sound(thing.c_str()));
}

bool Audio::remove_sound(const std::string& tag)
{
    return sounds.remove(tag);
}

void Audio::quick_play(const std::string& tag)
{
    quick_play(sounds.at(tag));
}

void Audio::quick_play(lib2d::Sound& sound)
{
    frog::assert_ptr( spare_channels.next() )->play(sound);
}

void Audio::play(const std::string& tag, lib2d::Channel& chn)
{
    if (chn.is_playing())
    {
        LOG("FADE");
        chn.fade_in(sounds.at(tag), 30);
    }
    else
        chn.play(sounds.at(tag));
}

void Audio::set_spare_channels(int count)
{
    spare_channels.resize(count);
}

} // namespace frog

#endif
