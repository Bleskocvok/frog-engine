#pragma once

#include "scene.hpp"
#include "engine_base.hpp"
#include "frog/utils/ptr.hpp"

#include <string>
#include <unordered_map>
#include <optional>
#include <utility>      // move


namespace frog
{

template<typename GameObj>
class scene_manager
{
public:
    using Scene = scene<GameObj>;
    using Engine = typename GameObj::Engine;

private:
    std::unordered_map<std::string, ptr<Scene>> scenes;

    std::string _current{ };

    std::optional<std::string> _next = std::nullopt;

    template <typename Self, typename Func>
    static void for_each_impl(Self& self, Func func)
    {
        if (self.empty())
            return;

        self.current().for_each_object(func);
    }

public:
    scene_manager() = default;

    bool empty() const { return _current.empty(); }

    // Scene* add(std::string name, ptr<Scene> sc);
    // bool remove(const std::string& name);
    // void cleanup(engine& eng);
    // void init(engine&);
    // void stable_update(engine&);
    // void end_update(engine&);
    // void frame_update(engine&);
    // void switch_scene(std::string name);

    const Scene& current() const { return *scenes.at(_current); }
          Scene& current()       { return *scenes.at(_current); }

    const Scene& at(const std::string& tag) const { return *scenes.at(tag); }
          Scene& at(const std::string& tag)       { return *scenes.at(tag); }

    template<typename Func>
    void for_each_object(Func func) const { for_each_impl(*this, func); }

    template<typename Func>
    void for_each_object(Func func)       { for_each_impl(*this, func); }


    Scene* add(std::string name, ptr<scene_manager::Scene> sc)
    {
        if (empty())
        {
            _current = name;
        }
        return scenes.emplace(std::move(name), std::move(sc)).first->second.get();
    }

    bool remove(const std::string& name)
    {
        return scenes.erase(name) > 0;
    }

    void cleanup(Engine& eng)
    {
        if (empty())
            return;

        current().cleanup(eng);
    }

    void init(Engine& eng)
    {
        // init _all_ scenes
        for (auto& sc : scenes)
        {
            sc.second->init(eng);
        }
    }

    void stable_update(Engine& eng)
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

    void end_update(Engine& eng)
    {
        if (empty())
            return;

        current().end_update(eng);
    }

    void frame_update(Engine& eng)
    {
        if (empty())
            return;

        current().frame_update(eng);
    }

    void switch_scene(std::string name)
    {
        _next = std::move(name);
    }
};


} // namespace frog

