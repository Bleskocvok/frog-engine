#pragma once

#include <iostream>

#define LOGX(x) frog::log_ln(#x, "→", x)
#define LOG(...) frog::log_ln(__VA_ARGS__ )

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

