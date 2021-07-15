#pragma once

#include "graphics/model.hpp"
#include "graphics/ui_element.hpp"
#include "script.hpp"
#include "utils/ptr.hpp"

#include <vector>
#include <algorithm>    // for_each
#include <string>


namespace frog
{


class engine;
struct script;


class game_object
{
    gl::model _model;

    bool _destroyed = false;

    std::vector<ptr<script>> scripts;

    std::vector<ptr<gl::ui_element>> _elements;

    template<typename Func>
    void for_each_script(Func func)
    {
        std::for_each(scripts.begin(), scripts.end(), [&](auto& ptr)
            {
                assert(ptr);
                func(*ptr);
            });
    }

    std::string _tag;

public:
    game_object() = default;

    script* add_script(ptr<script> script);

    bool remove_script(script* scr);

    gl::ui_element* add_element(ptr<gl::ui_element>);

    bool remove_element(gl::ui_element*);

    void init(engine&);

    void stable_update(engine&);

    void end_update(engine&);

    void frame_update(engine&);

    void destroyed(engine&);

    void destroy() { _destroyed = true; }

    bool is_destroyed() { return _destroyed; }

    const gl::model& model() const { return _model; }
          gl::model& model()       { return _model; }

    const std::string& tag() const { return _tag; }
          std::string& tag()       { return _tag; }

    const auto& elements() const { return _elements; }
          auto& elements()       { return _elements; }

    template<typename Script>
    Script* get_script()
    {
        for (auto& script : scripts)
        {
            if (auto* result = dynamic_cast<Script*>(script.get()))
            {
                return result;
            }
        }
        return nullptr;
    }

    template<typename Script, typename Func>
    void for_each_script_instanceof(Func func)
    {
        for (auto& sc : scripts)
        {
            if (auto* converted = dynamic_cast<Script*>(sc.get()))
            {
                func(*converted);
            }
        }
    }
};


} // namespace frog
