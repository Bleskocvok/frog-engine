#pragma once

#include <iostream>         // clog
#include <type_traits>      // is_same_v

#define LOG(...)      frog::log_ln(__VA_ARGS__)
#define LOG_FUNC(...) frog::log_ln("Log function:", __func__  __VA_OPT__(,) __VA_ARGS__)
// Just because it sounds funny.
#define FLOG(x)       frog::log_ln(#x, "→", x)

#define FROG_UNWRAPE(x, ...) "err", [](){ static_assert(false, "maximum ten arguments supported"); return 1; }()
#define FROG_UNWRAP9(x, ...) #x, x __VA_OPT__(, FROG_UNWRAPE( __VA_ARGS__ ))
#define FROG_UNWRAP8(x, ...) #x, x __VA_OPT__(, FROG_UNWRAP9( __VA_ARGS__ ))
#define FROG_UNWRAP7(x, ...) #x, x __VA_OPT__(, FROG_UNWRAP8( __VA_ARGS__ ))
#define FROG_UNWRAP6(x, ...) #x, x __VA_OPT__(, FROG_UNWRAP7( __VA_ARGS__ ))
#define FROG_UNWRAP5(x, ...) #x, x __VA_OPT__(, FROG_UNWRAP6( __VA_ARGS__ ))
#define FROG_UNWRAP4(x, ...) #x, x __VA_OPT__(, FROG_UNWRAP5( __VA_ARGS__ ))
#define FROG_UNWRAP3(x, ...) #x, x __VA_OPT__(, FROG_UNWRAP4( __VA_ARGS__ ))
#define FROG_UNWRAP2(x, ...) #x, x __VA_OPT__(, FROG_UNWRAP3( __VA_ARGS__ ))
#define FROG_UNWRAP1(x, ...) #x, x __VA_OPT__(, FROG_UNWRAP2( __VA_ARGS__ ))
#define FROG_UNWRAP0(x, ...) #x, x __VA_OPT__(, FROG_UNWRAP1( __VA_ARGS__ ))

#define LOGX(...) frog::log_vars(FROG_UNWRAP0(__VA_ARGS__))

#define FROG_VERBOSE() frog::log<false, 10>(std::clog, __FILE__, ":", __LINE__, " (", __func__, "): ")
#define LOGV(...)  ( FROG_VERBOSE(),  LOG(__VA_ARGS__) )
#define LOGVX(...) ( FROG_VERBOSE(), LOGX(__VA_ARGS__) )

namespace frog {

// Depth is to avoid looping forever for a cyclic Arg. (Arg.begin() leading back
// to itself would cause it.)
template<bool Space, int Depth, typename Out, typename Arg, typename ... Args>
void log(Out& out, Arg&& arg, Args&& ... args)
{
    if constexpr (Depth <= 0)
    {
        out << "...";
    }
    else if constexpr (std::is_same_v<std::decay_t<Arg>, bool>)
    {
        out << (arg ? "true" : "false");
    }
    else if constexpr (requires{ out << arg; })
    {
        out << arg;
    }
    else if constexpr (requires{ arg.first; arg.second; })
    {
        out << "< ";
        log<Space, Depth - 1>(out, arg.first);
        out << ", ";
        log<Space, Depth - 1>(out, arg.second);
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
                // TODO: Make sure this cannot loop forever.
                log<Space, Depth - 1>(out, *it);
            }
            out << " ]";
        }
        else
            out << "[]";
    }
    else
        []<bool kil = true>{ static_assert(!kil, "cannot log this type (Arg)"); };

    if constexpr (sizeof...(Args) != 0)
    {
        if constexpr (Space)
            out << " ";
        log<Space, Depth - 1>(out, std::forward<Args>(args)...);
    }
}


template<typename ... Args>
void log_ln(Args&& ... args)
{
    log<true, 10>(std::clog, std::forward<Args>(args)...);
    std::clog << "\n" << std::flush;
}


template<typename Val, typename... Args>
void log_vars_rec(const char* var, Val&& val, Args&&... args)
{
    log<true, 10>(std::clog, var, "→", val);

    if constexpr (sizeof...(Args) != 0)
    {
        log<false, 10>(std::clog, ", ");
        log_vars_rec(std::forward<Args>(args)...);
    }
}

template<typename... Args>
void log_vars(Args&&... args)
{
    log_vars_rec(std::forward<Args>(args)...);
    std::clog << "\n" << std::flush;
}

} // namespace frog
