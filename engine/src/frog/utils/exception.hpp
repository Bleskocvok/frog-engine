#pragma once

#include "string_builder.hpp"

#include <exception>        // exception
#include <string>           // string
#include <utility>          // forward
#include <type_traits>      // is_base_of_v, decay_t

#ifdef FROG_USE_STACKTRACE
    #include <version>
    #if __has_include(<stacktrace>)
        #if __cpp_lib_stacktrace >= 202011L
            #include <stacktrace>
            #define FROG_HAS_STACKTRACE
        #endif
    #endif
#endif

namespace frog {


struct error : std::exception
{
    template<typename Arg, typename... Args>
    static std::string mk_str(Arg&& arg, Args&&... args)
    {
        // TODO: if C++20
        // if constexpr (requires{ arg.what() })
        if constexpr (std::is_base_of_v<std::exception, std::decay_t<Arg>>)
        {
            return make_string(std::forward<Args>(args)..., ": ", arg.what());
        }
        else
        {
            return make_string(std::forward<Arg>(arg),
                               std::forward<Args>(args)...);
        }
    }

    std::string str;
    std::string stacktrace = "[no stacktrace]";

    template<typename... Args>
    error(Args&&... args) : str(mk_str(std::forward<Args>(args)...))
    {
#ifdef FROG_HAS_STACKTRACE
        stacktrace = std::to_string( std::stacktrace::current() );
#endif
    }

    // TODO: noexcept???
    const char* what() const noexcept override { return str.c_str(); }
};

struct game_error : error {

template<typename... Args>
explicit game_error(Args&&... args) : error(std::forward<Args>(args)...)
{ }

};

}  // namespace frog
