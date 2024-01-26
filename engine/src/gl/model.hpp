#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "material.hpp"
#include "geometry/matrix.hpp"
#include "geometry/vector.hpp"

#include <string>
#include <memory>       // unique_ptr


namespace frog::gl
{


struct model
{
    material mat = mats::white;
    std::string mesh_tag;
    std::string texture_tag;

    geo::vec3 pos;
    geo::vec3 rot;
    geo::vec3 scale = { 1 };

    geo::mat4 matrix() const;
};


} // namespace frog::gl

#endif
