#include "object_builder.hpp"

#include "engine.hpp"


using frog::ptr;
using frog::game_object;


void frog::object_builder::add_mesh(std::string mesh_tag)
{
    result->model().mesh_tag = std::move(mesh_tag);
}


void frog::object_builder::add_texture(std::string texture)
{
    result->model().texture_tag = std::move(texture);
    result->model().mat = gl::mats::texture_mat;
}


frog::gl::ui_element* frog::object_builder::add_element(ptr<gl::ui_element> el)
{
    return result->add_element(std::move(el));
}


void frog::object_builder::set_position(geo::vec3 pos)
{
    result->model().pos = pos;
}


void frog::object_builder::set_rotation(geo::vec3 rot)
{
    result->model().rot = rot;
}


void frog::object_builder::set_scale(geo::vec3 scale)
{
    result->model().scale = scale;
}


void frog::object_builder::set_material(gl::material mat)
{
    result->model().mat = std::move(mat);
}


void frog::object_builder::set_tag(std::string tag)
{
    result->tag() = std::move(tag);
}


void frog::object_builder::reset()
{
    result.reset();
    result = mk_ptr<game_object>();
}


ptr<game_object> frog::object_builder::build()
{
    auto res = std::move(result);
    reset();
    return res;
}


void frog::object_builder::build_in(engine& eng)
{
    eng.scenes->current().add(build());
}
