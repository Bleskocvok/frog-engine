#ifndef NOT_FROG_BUILD_2D

#include "engine2d.hpp"

#include "graphics/renderer2d.hpp"

#include <utility>      // std::move
#include <algorithm>    // std::find
#include <iterator>     // std::distance

using namespace frog::geo;
using namespace frog;


namespace frog {


engine2d::engine2d(settings set, ptr<state> _global)
    : engine_base( mk_ptr<lib2d::gx::window>(set.width, set.height, set.window_name.c_str()),
                   mk_ptr<gx::renderer2d>(),
                   std::move(_global) )
    , win_raw(static_cast<lib2d::gx::window*>(window.get()))
{
}


void engine2d::init()
{
    scenes->init(*this);
}


void engine2d::update_controls()
{

}


void engine2d::stable_update()
{}

void engine2d::frame_update()
{}


// unused parameter, TODO use it for extrapolation of movement
void engine2d::draw3D(double /* between */)
{

}


void engine2d::drawUI(double)
{

}


}  // namespace engine2d

#endif
