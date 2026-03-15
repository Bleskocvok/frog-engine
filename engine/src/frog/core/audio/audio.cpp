#ifndef NOT_FROG_BUILD_2D

#include "frog/core/audio/audio.hpp"

namespace frog {

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
