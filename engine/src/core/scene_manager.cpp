#include "scene_manager.hpp"

#include "engine.hpp"

#include <utility>      // move


using frog::scene;


scene* frog::scene_manager::add(std::string name, ptr<scene> sc)
{
    if (empty())
    {
        _current = name;
    }
    return scenes.emplace(std::move(name), std::move(sc)).first->second.get();
}


bool frog::scene_manager::remove(const std::string& name)
{
    return scenes.erase(name) > 0;
}


void frog::scene_manager::cleanup(frog::engine& eng)
{
    if (empty())
        return;

    current().cleanup(eng);
}


void frog::scene_manager::init(frog::engine& eng)
{
    // init _all_ scenes
    for (auto& sc : scenes)
    {
        sc.second->init(eng);
    }
}


void frog::scene_manager::stable_update(frog::engine& eng)
{
    if (empty())
        return;

    if (_next)
    {
        _current = std::move(*_next);
        _next.reset();
    }

    current().stable_update(eng);
}


void frog::scene_manager::end_update(frog::engine& eng)
{
    if (empty())
        return;

    current().end_update(eng);
}


void frog::scene_manager::frame_update(frog::engine& eng)
{
    if (empty())
        return;

    current().frame_update(eng);
}


void frog::scene_manager::switch_scene(std::string name)
{
    _next = std::move(name);
}



