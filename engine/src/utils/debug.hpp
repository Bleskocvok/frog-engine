#pragma once

#include <iostream>

namespace frog
{

template<typename Arg, typename ... Args>
inline void log_ln(Arg&& arg, Args&& ... args)
{
    if constexpr (sizeof...(Args) == 0)
    {
        std::clog << arg << '\n';
    }
    else
    {
        std::clog << arg << " ";
        log_ln(std::forward<Args>(args)...);
    }
}


} // namespace frog

