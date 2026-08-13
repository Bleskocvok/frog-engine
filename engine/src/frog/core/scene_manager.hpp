#pragma once

#include "frog/debug.hpp"
#include "frog/utils/assert.hpp"
#include "frog/utils/exception.hpp"
#include "scene.hpp"
#include "frog/utils/ptr.hpp"

#include <exception>
#include <stdexcept>
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

    template<typename S, typename Self>
    static S& current_impl(Self& self)
    {
        try
        {
            if (*self.inside > 0)
                return *self.scenes.at(self._current);
        }
        catch (...)
        { }

        return self.at(self._current);
    }

    template<typename S, typename Self>
    static S& at_impl(Self& self, const std::string& tag)
    {
        try
        {
            if (self.removed.contains(tag))
                return *self.added.at(tag);

            auto it = self.scenes.find(tag);
            if (it == self.scenes.end())
                return *self.added.at(tag);

            return *it->second;
        }
        catch (std::out_of_range& e)
        {
            throw frog::error("scene_manager: invalid scene ", tag);
        }
    }

public:
    scene_manager()
        : inside(std::make_shared<unsigned>(0))
    { }

    bool empty() const { return _current.empty(); }

    const Scene& current() const { return current_impl<const Scene&>(*this); }
          Scene& current()       { return current_impl<     Scene&>(*this); }

    const Scene& at(const std::string& tag) const { return at_impl<const Scene&>(*this, tag); }
          Scene& at(const std::string& tag)       { return at_impl<      Scene&>(*this, tag); }

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

    bool exists(const std::string& name) const
    {
        return scenes.contains(name);
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

        if (_next)
        {
            prev_next.emplace(_current, *_next);

            if (not empty())
            {
                auto it = scenes.find(_current);
                if (it != scenes.end())
                {
                    detail::SceneGuard g(inside);

                    it->second->deactivated(eng);
                }
            }
        }

        if (not removed.empty())
        {
            for (const auto& r : removed)
                scenes.erase(r);

            removed.clear();
        }
        if (not added.empty())
        {
            for (auto&& [k, v] : added)
            {
                auto [it, ok] = scenes.emplace(k, std::move(v));
                // frog_assert(ok);
                // it->init(eng);
            }

            added.clear();
        }

        detail::SceneGuard g(inside);

        if (_next)
        {
            prev_next.emplace(_current, *_next);

            // deactivated(eng);

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

