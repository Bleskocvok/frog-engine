#pragma once

#include "frog/utils/assert.hpp"
#include "scene.hpp"
#include "frog/utils/ptr.hpp"

#include <exception>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <optional>
#include <utility>      // move


namespace frog
{

namespace detail {

struct SceneGuard
{
    std::weak_ptr<unsigned> inside;

    SceneGuard(std::weak_ptr<unsigned> inside)
        : inside(inside)
    {
        if (std::shared_ptr<unsigned> b = inside.lock())
            ( *b )++;
        else
            frog_assert(false);
    }

    ~SceneGuard()
    {
        if (std::shared_ptr<unsigned> b = inside.lock())
            ( *b )--;
        else
            // TODO: Idk?
            std::terminate();
    }
};

} // namespace detail

template<typename GameObj>
class scene_manager
{
public:
    using Scene = scene<GameObj>;
    using Engine = typename GameObj::Engine;

private:
    std::unordered_map<std::string, ptr<Scene>> scenes;

    std::set<std::string> removed;
    std::map<std::string, ptr<Scene>> added;

    std::string _current{ };

    std::optional<std::string> _next = std::nullopt;

    std::optional<std::pair<std::string, std::string>> prev_next = std::nullopt;

    template <typename Self, typename Func>
    static void for_each_impl(Self& self, Func func)
    {
        if (self.empty())
            return;

        detail::SceneGuard g(self.inside);

        self.current().for_each_object(func);
    }

    void activated(Engine& eng)
    {
        if (empty())
            return;

        detail::SceneGuard g(inside);

        current().activated(eng);
    }

    void deactivated(Engine& eng)
    {
        if (empty())
            return;

        detail::SceneGuard g(inside);

        current().deactivated(eng);
    }

    std::shared_ptr<unsigned> inside;

    bool is_inside() const
    {
        frog_assert(inside);
        return *inside > 0;
    }

public:
    scene_manager()
        : inside(std::make_shared<unsigned>(0))
    { }

    bool empty() const { return _current.empty(); }

    const Scene& current() const { return at(_current); }
          Scene& current()       { return at(_current); }

    const Scene& at(const std::string& tag) const
    {
        if (removed.contains(tag))
            return *added.at(tag);

        auto it = scenes.find(tag);
        if (it == scenes.end())
            return *added.at(tag);

        return *it->second;
    }
          Scene& at(const std::string& tag)
    {
        if (removed.contains(tag))
            return *added.at(tag);

        auto it = scenes.find(tag);
        if (it == scenes.end())
            return *added.at(tag);

        return *it->second;
    }

    template<typename Func>
    void for_each_object(Func func) const { for_each_impl(*this, func); }

    template<typename Func>
    void for_each_object(Func func)       { for_each_impl(*this, func); }

    decltype(scenes)::const_iterator begin() const { return scenes.cbegin(); }
    decltype(scenes)::      iterator begin()       { return scenes.begin(); }

    decltype(scenes)::const_iterator end() const { return scenes.cend(); }
    decltype(scenes)::      iterator end()       { return scenes.end(); }

    const std::optional<std::pair<std::string, std::string>>& just_switched() const
    {
        return prev_next;
    }

    Scene* add(std::string name, ptr<scene_manager::Scene> sc)
    {
        if (empty())
        {
            _current = name;
        }
        auto* res = added.emplace(name, std::move(sc)).first->second.get();
        res->name = std::move(name);
        return res;
    }

    bool remove(const std::string& name)
    {
        // frog_assert(not is_inside() || _current != name);
        // return scenes.erase(name) > 0;
        bool has = scenes.contains(name);
        if (has)
            removed.insert(name);
        return has;
    }

    void cleanup(Engine& eng)
    {
        if (empty())
            return;

        detail::SceneGuard g(inside);

        current().cleanup(eng);
    }

    void init(Engine& eng)
    {
        if (empty())
            return;

        detail::SceneGuard g(inside);

        current().init(eng);

        // TOOD: Son of a bitch, gotta go figure out if this broke something.
        // // init _all_ scenes
        // for (auto& sc : scenes)
        // {
        //     sc.second->init(eng);
        // }
    }

    void stable_update(Engine& eng)
    {
        if (empty())
            return;

        detail::SceneGuard g(inside);

        if (not removed.empty())
        {
            for (const auto& r : removed)
                scenes.erase(r);

            removed.clear();
        }
        if (not added.empty())
        {
            for (auto&& [k, v] : added)
                scenes.emplace(k, std::move(v));

            added.clear();
        }

        if (_next)
        {
            prev_next.emplace(_current, *_next);

            deactivated(eng);

            _current = std::move(*_next);
            _next.reset();

            activated(eng);
        }

        current().stable_update(eng);

        if (prev_next)
            prev_next.reset();
    }

    void pre_update(Engine& eng)
    {
        detail::SceneGuard g(inside);

        if (empty())
            return;

        current().pre_update(eng);
    }

    void end_update(Engine& eng)
    {
        detail::SceneGuard g(inside);

        if (empty())
            return;

        current().end_update(eng);
    }

    void frame_update(Engine& eng)
    {
        detail::SceneGuard g(inside);

        if (empty())
            return;

        current().frame_update(eng);
    }

    void end_frame_update(Engine& eng)
    {
        detail::SceneGuard g(inside);

        if (empty())
            return;

        current().end_frame_update(eng);
    }

    void switch_scene(std::string name)
    {
        _next = std::move(name);
    }
};


} // namespace frog

