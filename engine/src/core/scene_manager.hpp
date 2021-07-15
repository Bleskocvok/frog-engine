#pragma once

#include "scene.hpp"
#include "utils/ptr.hpp"

#include <string>
#include <unordered_map>
#include <optional>


namespace frog
{


class engine;


class scene_manager
{
    std::unordered_map<std::string, ptr<scene>> scenes;

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

    scene* add(std::string name, ptr<scene> sc);

    bool remove(const std::string& name);

    bool empty() const { return _current.empty(); }

    void cleanup(engine& eng);

    void init(engine&);

    void stable_update(engine&);

    void end_update(engine&);

    void frame_update(engine&);

    void switch_scene(std::string name);

    const scene& current() const { return *scenes.at(_current); }
          scene& current()       { return *scenes.at(_current); }

    const scene& at(const std::string& tag) const { return *scenes.at(tag); }
          scene& at(const std::string& tag)       { return *scenes.at(tag); }

    template<typename Func>
    void for_each_object(Func func) const { for_each_impl(*this, func); }

    template<typename Func>
    void for_each_object(Func func)       { for_each_impl(*this, func); }
}; 


} // namespace frog

