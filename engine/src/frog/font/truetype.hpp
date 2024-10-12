#pragma once

#include "font_base.hpp"

#include <string>


namespace frog::font {


class truetype : public base
{
public:
    truetype(std::string file) {}
};


} // namespace frog
