#pragma once

#include "exception.hpp"

#define frog_assert( expr ) \
    do {                    \
        if ( !( expr )) \
            throw frog::error( "assert failed ", __FILE__, ":", __LINE__,   \
                            ": frog_assert( ", #expr, " )" );               \
    } while (false)
