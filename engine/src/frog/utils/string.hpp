#pragma once

#include <vector>       // vector
#include <string>       // string
#include <string_view>  // string_view
#include <cstddef>      // size_t

namespace frog
{

std::vector<std::string> split(std::string_view str, std::string_view delim = "\n");

void trim(std::string_view& str);
void trim(std::string& str);

std::string_view next_segment(std::string_view& str, std::string_view delim = "\n");

template<typename Func>
void for_each_segment(std::string_view str, std::string_view delim, Func&& func)
{
    std::size_t prev = 0;
    std::size_t found = str.find(delim);
    while (found != decltype(str)::npos)
    {
        std::string_view segment = str.substr(prev, found - prev);
        prev = found + 1;
        found = str.find(delim, prev);
        func(segment);
    }
    std::string_view segment = str.substr(prev);
    func(segment);
}

} // namespace frog
