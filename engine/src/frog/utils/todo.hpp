#pragma once

#include "exception.hpp"

#define TODO( ... ) \
    throw frog::error( "not implemented ", __FILE__, ":", __LINE__, ":" __VA_OPT__(,) __VA_ARGS__ );
