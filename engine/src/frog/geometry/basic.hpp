#pragma once

#include <cmath>        // acos, abs


namespace frog::geo
{


static constexpr float Epsilon = 1e-6;
inline static float Pi = std::acos(-1);
inline static float ToRad = Pi / 180.0f;
inline static float ToDeg = 180.0f / Pi;


inline bool is_close(float a, float b)
{
    return std::abs(a - b) < Epsilon;
}


} // namespace frog::geo
