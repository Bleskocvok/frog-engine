#pragma once

#include <iostream>         // clog
#include <type_traits>      // is_same_v
#include <string>           // string

#define LOGX(x) frog::log_ln(#x, "→", x)
#define LOG(...) frog::log_ln(__VA_ARGS__ )


namespace frog
{


template<typename Arg, typename ... Args>
void log(Arg&& arg, Args&& ... args)
{
    if constexpr (std::is_same_v<std::decay_t<Arg>, std::string>)
    {
        std::clog << arg;
    }
    else if constexpr (requires{ arg.begin(); arg.end(); })
    {
        if (arg.begin() != arg.end())
        {
            std::clog << "[ ";
            const char* del = "";
            for (auto it = arg.begin(), end = arg.end(); it != end; ++it)
            {
                std::clog << del;
                del = ", ";
                log(*it);
            }
            std::clog << " ]";
        }
        else
            std::clog << "[]";
    }
    else
        std::clog << arg;

    if constexpr (sizeof...(Args) != 0)
    {
        std::clog << " ";
        log(std::forward<Args>(args)...);
    }
}


template<typename ... Args>
void log_ln(Args&& ... args)
{
    log(std::forward<Args>(args)...);
    std::clog << "\n";
}


} // namespace frog

