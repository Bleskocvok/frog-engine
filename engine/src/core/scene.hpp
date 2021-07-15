#pragma once

#include "game_object.hpp"
#include "utils/ptr.hpp"

#include <memory>       // unique_ptr
#include <utility>      // move
#include <algorithm>    // remove_if, for_each, find_if
#include <vector>
#include <string_view>


namespace frog
{


class engine;


inline static auto all = [](const auto&){ return true; };

inline static auto by_tag(std::string_view tag)
{
    return [=](const auto& obj){ return obj.tag() == tag; };
}


class scene
{
    std::vector<ptr<game_object>> objects;
    std::vector<ptr<game_object>> to_add;

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

    void add_objects(engine&);

public:

    void add(ptr<game_object> obj)
    {
        to_add.push_back(std::move(obj));
    }

    void init(engine&);

    void stable_update(engine&);

    void end_update(engine&);

    void frame_update(engine&);

    void cleanup(engine&);

    /*
     * Functions to iterate or get objects
     */

    template<typename Predicate>
    game_object* get_object(Predicate pred)
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

    template<typename Script, typename Predicate, typename Func>
    void for_each_script(Predicate pred, Func func)
    {
        for_each_object(pred, [&](game_object& obj)
        {
            auto* script = obj.get_script<Script>();
            if (script)
            {
                func(*script);
            }
        });
    }

    template<typename Script, typename Func>
    void for_each_script(Func func) { for_each_script<Script>(all, func); }
};


} // namespace frog
