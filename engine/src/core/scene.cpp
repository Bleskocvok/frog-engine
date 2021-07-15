#include "scene.hpp"

#include "game_object.hpp"
#include "engine.hpp"


void frog::scene::cleanup(frog::engine& eng)
{
    for (auto& ptr : objects)
    {
        if (ptr->is_destroyed())
        {
            ptr->destroyed(eng);
            ptr.reset();
        }
    }

    objects.erase(std::remove_if(
                    objects.begin(),
                    objects.end(),
                    [&](const auto& ptr) { return !ptr; }),
                objects.end());

    add_objects(eng);
}


void frog::scene::add_objects(frog::engine& eng)
{
    for (size_t i = 0; i < to_add.size(); i++)
    {
        if (!to_add[i])
            continue;

        to_add[i]->init(eng);
        objects.push_back(std::move(to_add[i]));
    }
    to_add.clear();
}


void frog::scene::init(frog::engine& eng)
{
    add_objects(eng);
}


void frog::scene::stable_update(frog::engine& eng)
{
    for_each_object([&](auto& obj) { obj.stable_update(eng); });
}


void frog::scene::end_update(frog::engine& eng)
{
    for_each_object([&](auto& obj) { obj.end_update(eng); });
}


void frog::scene::frame_update(frog::engine& eng)
{
    for_each_object([&](auto& obj) { obj.frame_update(eng); });
}
