#include "camera.hpp"

#include "frog/geometry/transform.hpp"


using namespace frog;
using namespace frog::geo;


void camera::recalculate()
{
    _projection = geo::perspective(_fov, _ratio, _near, _far);
}


void camera::look_from(geo::vec3 pos, float y_rot, float x_rot)
{
    _view = rotate(y_axis{}, -y_rot)
            * rotate(x_axis{}, -x_rot)
            * translate(-pos);
    _eye_pos = pos;
}


void camera::look_towards(geo::vec3 pos, float dist, float y_rot, float x_rot)
{
    _view = translate({ 0, 0, -dist })
            * rotate(y_axis{}, -y_rot)
            * rotate(x_axis{}, -x_rot)
            * translate(-pos);
    auto eye = rotate(x_axis{}, -x_rot)
            * rotate(y_axis{}, -y_rot)
            * translate({ 0, 0, -dist })
            * vec4(pos, 1);
    float div = 1 / eye.w();
    _eye_pos = { eye.x(), eye.y(), eye.z() };
    _eye_pos *= div;
}
