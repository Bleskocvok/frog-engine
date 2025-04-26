#include "exception.hpp"

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

void error::init_stacktrace()
{
#ifdef FROG_HAS_STACKTRACE
    stacktrace = std::to_string( std::stacktrace::current() );
#endif
}

} // namespace frog
