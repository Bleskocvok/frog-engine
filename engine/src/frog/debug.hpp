#pragma once

#include <iostream>         // clog
#include <type_traits>      // is_same_v
#include <string>           // string
#include <ostream>          // ostream

#define LOGX(x) frog::log_ln(#x, "→", x)
#define LOG(...) frog::log_ln(__VA_ARGS__)
#define LOG_FUNC(...) frog::log_ln("Log function:", __func__  __VA_OPT__(,) __VA_ARGS__)


namespace frog
{


template<typename Arg, typename ... Args>
void log(std::ostream& out, Arg&& arg, Args&& ... args)
{
    if constexpr (std::is_same_v<std::decay_t<Arg>, bool>)
    {
        out << (arg ? "true" : "false");
    }
    else if constexpr (std::is_same_v<std::decay_t<Arg>, std::string>)
    {
        out << arg;
    }
    else if constexpr (requires{ arg.first; arg.second; })
    {
        out << "< ";
        log(out, arg.first);
        out << ", ";
        log(out, arg.second);
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
                log(out, *it);
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
        log(out, std::forward<Args>(args)...);
    }
}


template<typename ... Args>
void log_ln(Args&& ... args)
{
    log(std::clog, std::forward<Args>(args)...);
    std::clog << "\n" << std::flush;
}


} // namespace frog

