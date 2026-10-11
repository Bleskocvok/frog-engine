#ifndef NOT_FROG_BUILD_2D

#include "mixer_init.hpp"

namespace frog::lib2d {

InitSound::InitSound(std::uint32_t flags)
{
    int libflags = 0;

    if (flags & Format::OGG)
        libflags |= MIX_INIT_OGG;

    if ((Mix_Init(libflags) & libflags) != libflags)
        throw std::runtime_error(Mix_GetError());
}

InitSound::~InitSound()
{
    Mix_Quit();
}

namespace detail {

void SoundDeleter::operator()(Mix_Chunk* obj) { Mix_FreeChunk(obj); }
void SoundDeleter::operator()(Mix_Music* obj) { Mix_FreeMusic(obj); }

} // namespace detail

} // namespace frog::lib2d

#endif
