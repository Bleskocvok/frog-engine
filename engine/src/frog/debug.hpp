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
    auto& out = std::clog;

    if constexpr (std::is_same_v<std::decay_t<Arg>, std::string>)
    {
        out << arg;
    }
    else if constexpr (requires{ arg.first; arg.second; })
    {
        out << "< ";
        log(arg.first);
        out << ", ";
        log(arg.second);
        out << " >";
    }
    else if constexpr (requires{ arg.begin(); arg.end(); })
    {
        if (arg.begin() != arg.end())
        {
            out << "[ ";
            const char* del = "";
            for (auto it = arg.begin(), end = arg.end(); it != end; ++it)
            {
                out << del;
                del = ", ";
                log(*it);
            }
            out << " ]";
        }
        else
            out << "[]";
    }
    else
        out << arg;

    if constexpr (sizeof...(Args) != 0)
    {
        out << " ";
        log(std::forward<Args>(args)...);
    }
}


template<typename ... Args>
void log_ln(Args&& ... args)
{
    log(std::forward<Args>(args)...);
    std::clog << "\n" << std::flush;
}


} // namespace frog

