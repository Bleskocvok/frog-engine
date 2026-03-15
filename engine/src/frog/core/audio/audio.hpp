#pragma once

#include "frog/graphics/assets.hpp"
#include "frog/lib2d/audio/audio.hpp"
#include "frog/lib2d/audio/sound.hpp"
#include "frog/utils/ptr.hpp"

#include <string>
#include <utility>

namespace frog {

class Audio
{
    lib2d::InitSound init_sound = lib2d::InitSound( lib2d::Format::OGG );

    std::string path_prefix;

    lib2d::Audio audio;

    gx::assets<lib2d::Sound> sounds;

    static constexpr int FREQUENCY = 22050;
    static constexpr int CHANNELS = 2;
    static constexpr int CHUNK_SIZE = 1024;

public:
    Audio(std::string path_prefix)
        : path_prefix(std::move(path_prefix))
        , audio(FREQUENCY, CHANNELS, CHUNK_SIZE)
    { }

    bool add_sound(const std::string& tag, const std::string& path);

    bool remove_sound(const std::string& tag);

    void quick_play(const std::string& tag);

    void quick_play(lib2d::Sound& sound);
};

} // namespace frog
