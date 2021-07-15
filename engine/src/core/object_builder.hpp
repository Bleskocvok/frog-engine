#pragma once

#include "game_object.hpp"
#include "utils/ptr.hpp"


namespace frog
{


class engine;


class object_builder
{
    ptr<game_object> result = mk_ptr<game_object>();

public:

    void add_mesh(std::string mesh_tag);

    void add_script(ptr<script> script)
    {
        result->add_script(std::move(script));
    }

    template<typename Script, typename ... Args>
    void emplace_script(Args&& ... args)
    {
        add_script(mk_ptr<Script>(std::forward<Args>(args)...));
    }

    gl::ui_element* add_element(ptr<gl::ui_element>);

    void set_tag(std::string tag);

    void set_position(geo::vec3 pos);

    void set_rotation(geo::vec3 rot);

    void set_scale(geo::vec3 scale);

    void add_texture(std::string texture);

    void set_material(gl::material mat);

    void reset();

    ptr<game_object> build();

    void build_in(engine&);
};


} // namespace frog
