#pragma once

#include "string_builder.hpp"

#include <exception>        // exception
#include <string>           // string
#include <utility>          // forward
#include <type_traits>      // is_base_of_v, decay_t


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

    template<typename... Args>
    error(Args&&... args) : str(mk_str(std::forward<Args>(args)...))
    { }

    // TODO: noexcept???
    const char* what() const noexcept override { return str.c_str(); }
};


}  // namespace frog
