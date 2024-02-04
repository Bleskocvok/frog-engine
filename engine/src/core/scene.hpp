#pragma once

#include "game_object_base.hpp"
#include "utils/ptr.hpp"
#include "engine_base.hpp"

#include <memory>       // unique_ptr
#include <utility>      // move
#include <algorithm>    // remove_if, for_each, find_if
#include <vector>
#include <string_view>


namespace frog {


inline static auto all = [](const auto&){ return true; };

inline static auto by_tag(std::string_view tag)
{
    return [=](const auto& obj){ return obj.tag() == tag; };
}


template<typename GameObject>
class scene
{
public:
    using Engine = typename GameObject::Engine;

    std::vector<ptr<GameObject>> objects;
    std::vector<ptr<GameObject>> to_add;

    template <typename Self, typename Func, typename Predicate>
    static void for_each_impl(Self& self, Func func, Predicate pred)
    {
        std::for_each(self.objects.begin(), self.objects.end(),
            [=](auto& ptr)
            {
                if (ptr && pred(*ptr))
                    func(*ptr);
            });
    }

    template<typename Engine>
    void add_objects(Engine&);

public:
    void add(ptr<GameObject> obj)
    {
        to_add.push_back(std::move(obj));
    }

    /*
     * Functions to iterate or get objects
     */

    template<typename Predicate>
    GameObject* get_object(Predicate pred)
    {
        auto found = std::find_if(objects.begin(), objects.end(),
            [&](auto& ptr) { return ptr && pred(*ptr); });

        if (found == objects.end())
            return nullptr;
        return found->get();
    }

    template<typename Func>
    void for_each_object(Func func) const { for_each_impl(*this, func, all); }

    template<typename Func>
    void for_each_object(Func func)       { for_each_impl(*this, func, all); }

    template<typename Predicate, typename Func>
    void for_each_object(Predicate pred, Func func) const
    {
        for_each_impl(*this, func, pred);
    }

    template<typename Predicate, typename Func>
    void for_each_object(Predicate pred, Func func)
    {
        for_each_impl(*this, func, pred);
    }

    template<typename S, typename Predicate, typename Func>
    void for_each_script(Predicate pred, Func func)
    {
        auto f = [&](GameObject& o)
        {
            auto* script = go_get_script<GameObject, S>(o);
            if (script)
                func(*script);
        };

        for_each_object(pred, f);
    }

    template<typename Script, typename Func>
    void for_each_script(Func func) { for_each_script<Script>(all, func); }


    void cleanup(Engine& eng)
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

    void add_objects(Engine& eng)
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

    void init(Engine& eng)
    {
        add_objects(eng);
    }

    void stable_update(Engine& eng)
    {
        for_each_object([&](auto& obj) { obj.stable_update(eng); });
    }

    void end_update(Engine& eng)
    {
        for_each_object([&](auto& obj) { obj.end_update(eng); });
    }

    void frame_update(Engine& eng)
    {
        for_each_object([&](auto& obj) { obj.frame_update(eng); });
    }
};


} // namespace frog
