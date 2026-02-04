#pragma once

#include "exception.hpp"

#include <filesystem>
#include <source_location>

namespace frog {

inline std::string basename(const std::string& path)
{
    if (path.find('/') != path.npos)
        return std::filesystem::path(path).filename().string();

    return path;
}

template< typename T >
T* assert_ptr(T* ptr, const std::source_location loc = std::source_location::current())
{
    if ( not ptr )
            throw frog::error( "assert failed ", frog::basename( loc.file_name() ), ":", loc.line() );
    return ptr;
}

} // namespace frog

// This one stays even with NDEBUG
#define frog_assert( expr ) \
    do {                    \
        if ( not ( expr )) \
            throw frog::error( "assert failed ", frog::basename( __FILE__ ), ":", __LINE__,   \
                            ": frog_assert( ", #expr, " )" );               \
    } while (false)
