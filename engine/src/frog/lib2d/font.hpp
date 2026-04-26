#pragma once

#include "structs.hpp"

#include <string>       // string
#include <utility>      // pair
#include <cstdint>      // uint8_t


namespace frog::lib2d {


class font
{
    detail::Font font_;
public:
    font(const std::string& path, int size);

    detail::Surface render_text(const std::string& text, std::uint8_t r,
                                std::uint8_t g, std::uint8_t b,
                                std::uint8_t a = 255);

    detail::Surface render_text(const std::string& text);

    std::pair<int, int> size_text(const std::string& text);

    void set_outline(int size);
};


}  // namespace frog::lib2d
