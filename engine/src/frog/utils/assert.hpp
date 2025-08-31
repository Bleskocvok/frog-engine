#pragma once

#include "exception.hpp"

// This one stays even with NDEBUG
#define frog_assert( expr ) \
    do {                    \
        if ( not ( expr )) \
            throw frog::error( "assert failed ", __FILE__, ":", __LINE__,   \
                            ": frog_assert( ", #expr, " )" );               \
    } while (false)
