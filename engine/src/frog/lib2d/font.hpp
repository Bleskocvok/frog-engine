#pragma once

#include "structs.hpp"

#include <stdexcept>
#include <string>


namespace frog::lib2d {


class font
{
    detail::font font_;
public:
    font(const std::string& path, int size);

    detail::surface render_text(const std::string& text);
};


}  // namespace frog::lib2d
