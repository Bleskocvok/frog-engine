#pragma once

#include "geometry/matrix.hpp"
#include "geometry/vector.hpp"

#include <utility>      // move


namespace frog
{


class camera
{
    float _fov = 90;
    float _ratio = 1;
    float _near = 0.01;
    float _far = 100;

    geo::mat4 _projection{ 1 };
    geo::mat4 _view{ 1 };

    geo::vec3 _eye_pos;

    void recalculate();

public:

    camera() { recalculate(); }

    camera(float fov
         , float near = 0.01
         , float far = 100)
         : _fov(fov), _ratio(1), _near(near), _far(far)
    {
        recalculate();
    }

    void look_from(geo::vec3 pos, float y_rot, float x_rot);

    void look_towards(geo::vec3 pos, float dist, float y_rot, float x_rot);

    geo::vec3 eye_position() const { return _eye_pos; }

    const geo::mat4& projection() const { return _projection; }

    const geo::mat4& view() const { return _view; }

    void view(geo::mat4 mat) { _view = std::move(mat); }

    void fov(float val)
    {
        _fov = val;
        recalculate();
    }

    void screen_size(float w, float h)
    {
        _ratio = w / h;
        recalculate();
    }

    void cutout(float near, float far)
    {
        _near = near;
        _far = far;
        recalculate();
    }

    float ratio() const { return _ratio; }
};


} // namespace frog

