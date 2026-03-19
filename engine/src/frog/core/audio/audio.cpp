#ifndef NOT_FROG_BUILD_2D

#include "frog/core/audio/audio.hpp"
#include "frog/utils/todo.hpp"

#include <vector>
#include <random>

namespace frog {

bool Group::add_sound(const std::string& tag, const std::string& path)
{
    // TODO: What if sound of tag exists but is different path?
    // Exception?
    sounds.push_back(tag);
    return audio->add_sound(tag, path);
}

bool Group::remove_sound(const std::string& tag)
{
    std::erase(sounds, tag);
    return audio->remove_sound(tag);
}

void Group::quick_play()
{
    if (sounds.empty())
        return;

    static std::random_device rd{};
    static thread_local std::mt19937 gen{ rd() };

    auto dist = std::uniform_int_distribution<size_t>(0, sounds.size() - 1);

    audio->quick_play(sounds[ dist(gen) ]);
}

void Group::play(lib2d::Channel& chn)
{
    TODO();
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
    auto& chn = audio.add_channel();
    chn.play(sound);
}

} // namespace frog

#endif
