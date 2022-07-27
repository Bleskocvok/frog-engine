#pragma once

#include "assets.hpp"
#include "scene.hpp"


namespace frog
{


class os_context
{
    void draw_sprite(gl::program& ui,
                     const std::string& texture,
                     geo::vec2 pos,
                     geo::vec2 size,
                     geo::vec2 tex_pos,
                     geo::vec2 tex_size,
                     geo::vec4 color);

    void draw_text(gl::program& ui,
                   std::string_view str,
                   geo::vec2 pos,
                   float height,
                   geo::vec4 color,
                   bool centered);
public:
    /*
     * Asset collections
     */
    gl::assets<gl::mesh> meshes;
    gl::assets<gl::program> programs;
    gl::assets<gl::texture> textures;

    os_context() = default;

    /*
     * Other OS features
     */

    //void set_callbacks(input* in);

    //void poll_events();

    //void resize(int w, int h);

    /*
     * Rendering
     */

    void clear_color(float r, float g, float b);

    void clear();

    //void start_3D();

    //void start_ui();

    //void draw_object(const model& obj, double between);

    //void draw_object(const ui_element& obj, double between);

    void draw(const scene&);

    void swap_buffers();
};


} // namespace frog
