#ifndef NOT_FROG_BUILD_3D

#pragma once

#include "program.hpp"
#include "location.hpp"
#include "geometry/vector.hpp"


namespace frog::gl
{


struct material
{
    // .x() = R; .y() = G; .z() = B
    geo::vec4 ambient   { 0.1, 0.1, 0.1, 1 };   // default colors: grayish
    geo::vec4 diffuse   { 0.4, 0.4, 0.4, 1 };
    geo::vec4 specular  { 0.8, 0.8, 0.8, 2 };   // .w() = shininess


    void send(program& pr, location::material loc) const
    {
        pr.uniform_vec(ambient, loc.ambient);
        pr.uniform_vec(diffuse, loc.diffuse);
        pr.uniform_vec(specular, loc.specular);
    }
};


namespace mats
{


inline static const material grayish =
{
    { 0.1, 0.1, 0.1,  1 },
    { 0.4, 0.4, 0.4,  1 },
    { 0.8, 0.8, 0.8, 20 },
};


inline static const material white =
{
    { 0.1, 0.1, 0.1,  1 },
    { 1,   1,   1,    1 },
    { 0,   0,   0,    1 },
};


inline static const material texture_mat =
{
    { 0,   0,   0,   1 },
    { 1,   1,   1,   1 },
    { 0,   0,   0,   1 },
};


} // namespace mats


} // namespace frog::gl

#endif
