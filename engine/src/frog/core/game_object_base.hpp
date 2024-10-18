#pragma once

#include "frog/graphics/ui_element.hpp"
#include "frog/utils/ptr.hpp"

#include <vector>
#include <algorithm>    // for_each
#include <string>
#include <cassert>
#include <utility>      // forward, utility


namespace frog
{


template<typename GameObject, typename Script>
static Script* go_get_script(GameObject& o)
{
    for (auto& script : o.get_scripts())
        if (auto* result = dynamic_cast<Script*>(script.get()))
            return result;
    return nullptr;
}


// Derived = CRTP
template<typename Derived, typename Script, typename Engine>
class game_object_base
{
private:
    bool _destroyed = false;

    std::vector<ptr<Script>> scripts;
    std::vector<ptr<gx::ui_element>> _elements;

    std::string _tag;

    template<typename Func>
    void for_each_script(Func func)
    {
        std::for_each(scripts.begin(), scripts.end(), [&](auto& ptr)
            {
                assert(ptr);
                func(*ptr);
            });
    }

    const Derived& get() const  { return *static_cast<const Derived*>(this); }
          Derived& get()        { return *static_cast<      Derived*>(this); }

public:
    virtual ~game_object_base() = default;

    // Script* add_script(ptr<Script> script);
    // bool remove_script(Script* scr);
    // gx::ui_element* add_element(ptr<gx::ui_element>);
    // bool remove_element(gx::ui_element*);
    // void init(Engine&) = 0;
    // void stable_update(Engine&);
    // void end_update(Engine&);
    // void frame_update(Engine&);
    // void destroyed(Engine&);

    void destroy() { _destroyed = true; }

    bool is_destroyed() { return _destroyed; }

    const std::string& tag() const { return _tag; }
          std::string& tag()       { return _tag; }

    const auto& elements() const { return _elements; }
          auto& elements()       { return _elements; }



    template<typename S>
    S* get_script()
    {
        for (auto& script : scripts)
            if (auto* result = dynamic_cast<S*>(script.get()))
                return result;

        return nullptr;
    }

    std::vector<ptr<Script>>& get_scripts() { return scripts; }

    template<typename S, typename Func>
    void for_each_script_instanceof(Func func)
    {
        for (auto& sc : scripts)
            if (auto* converted = dynamic_cast<S*>(sc.get()))
                func(*converted);
    }

    Script* add_script(ptr<Script> scr)
    {
        scripts.push_back(std::move(scr));
        return scripts.back().get();
    }

    template<typename T, typename... Args>
    T* mk_script(Args&&... args)
    {
        auto ptr = mk_ptr<T>(std::forward<Args>(args)...);
        T* raw = ptr.get();
        add_script(std::move(ptr));
        return raw;
    }

    bool remove_script(Script* scr)
    {
        auto rem_it = std::remove_if(scripts.begin(), scripts.end(),
            [=](const auto& ptr) { return ptr.get() == scr; });

        bool result = rem_it != scripts.end();

        scripts.erase(rem_it, scripts.end());

        return result;
    }

    gx::ui_element* add_element(ptr<gx::ui_element> el)
    {
        _elements.push_back(std::move(el));
        return _elements.back().get();
    }

    bool remove_element(gx::ui_element* elem)
    {
        auto rem_it = std::remove_if(_elements.begin(), _elements.end(),
            [=](const auto& ptr) { return ptr.get() == elem; });

        bool result = rem_it != _elements.end();

        _elements.erase(rem_it, _elements.end());

        return result;
    }

    void init(Engine& engine)
    {
        for_each_script([&](auto& sc){ sc.init(get(), engine); });
    }

    void stable_update(Engine& engine)
    {
        for_each_script([&](auto& sc){ sc.stable_update(get(), engine); });
    }

    void end_update(Engine& engine)
    {
        for_each_script([&](auto& sc) { sc.end_update(get(), engine); });
    }

    void frame_update(Engine& engine)
    {
        for_each_script([&](auto& sc){ sc.frame_update(get(), engine); });
    }

    void destroyed(Engine& engine)
    {
        for_each_script([&](auto& sc){ sc.destroyed(get(), engine); });
    }
};


} // namespace frog
