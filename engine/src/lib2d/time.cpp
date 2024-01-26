#ifndef NOT_FROG_BUILD_2D

#include "core.hpp"

#include <chrono>
#include <thread>


namespace frog::lib2d::os
{


void wait( long ms )
{
    std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
}


void timer::reset()
{
    // point = SDL_GetTicks();
}


std::uint64_t timer::get_ms() const
{
    // return SDL_GetTicks64() - point;
    return 0;
}


std::uint64_t timer::reset_ms()
{
    auto old = point;
    reset();
    return point - old;
}


unsigned timer::duration_us() const
{
    return get_ms() * 1000;
}


unsigned timer::reset_duration_us()
{
    return reset_ms() * 1000;
}


}  // namespace frog::lib2d::gx


#endif
