#pragma once

#include "exception.hpp"

#include <filesystem>

namespace frog {


inline std::string basename(const std::string& path)
{
    if (path.find('/') != path.npos)
        return std::filesystem::path(path).filename().string();

    return path;
}

} // namespace frog

// This one stays even with NDEBUG
#define frog_assert( expr ) \
    do {                    \
        if ( not ( expr )) \
            throw frog::error( "assert failed ", frog::basename( __FILE__ ), ":", __LINE__,   \
                            ": frog_assert( ", #expr, " )" );               \
    } while (false)
