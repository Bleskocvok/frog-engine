#include "game_object.hpp"

#include "script.hpp"

#include <utility>      // move
#include <algorithm>    // remove_if


using frog::script;


script* frog::game_object::add_script(ptr<script> scr)
{
    scripts.push_back(std::move(scr));
    return scripts.back().get();
}


bool frog::game_object::remove_script(script* scr)
{
    auto rem_it = std::remove_if(scripts.begin(), scripts.end(),
        [=](const auto& ptr) { return ptr.get() == scr; });

    bool result = rem_it != scripts.end();

    scripts.erase(rem_it, scripts.end());

    return result;
}


frog::gl::ui_element* frog::game_object::add_element(ptr<frog::gl::ui_element> el)
{
    _elements.push_back(std::move(el));
    return _elements.back().get();
}


bool frog::game_object::remove_element(frog::gl::ui_element* elem)
{
    auto rem_it = std::remove_if(_elements.begin(), _elements.end(),
        [=](const auto& ptr) { return ptr.get() == elem; });

    bool result = rem_it != _elements.end();

    _elements.erase(rem_it, _elements.end());

    return result;
}


void frog::game_object::init(engine& engine)
{
    for_each_script([&](auto& sc){ sc.init(*this, engine); });
}


void frog::game_object::stable_update(engine& engine)
{
    for_each_script([&](auto& sc){ sc.stable_update(*this, engine); });
}


void frog::game_object::end_update(engine& engine)
{
    for_each_script([&](auto& sc) { sc.end_update(*this, engine); });
}


void frog::game_object::frame_update(engine& engine)
{
    for_each_script([&](auto& sc){ sc.frame_update(*this, engine); });
}


void frog::game_object::destroyed(engine& engine)
{
    for_each_script([&](auto& sc){ sc.destroyed(*this, engine); });
}
