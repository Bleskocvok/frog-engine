#include "exception.hpp"

#include <memory>

#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>     // abi::__cxa_demangle
    #define FROG_HAS_DEMANGLE
#endif

namespace {

struct Free
{
    void operator()(char** ptr)
    {
        std::free(ptr);
    }

    void operator()(char* ptr)
    {
        std::free(ptr);
    }
};

bool demangle(std::string& mangled)
{
#ifndef FROG_HAS_DEMANGLE
    return true;
#else
    int status = 0;
    auto demangled = std::unique_ptr<char[], Free>{};
    demangled.reset( abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status) );

    static constexpr int OK = 0;
    static constexpr int INVALID_MANGLE = -2;

    if (status != OK)
        return false;

    mangled = std::string(demangled.get());
    return true;
#endif
}

} // namespace

#ifdef FROG_USE_STACKTRACE
    #include <version>
    #if __has_include(<stacktrace>)
        #include <stacktrace>
        #pragma message("ahoj")
        #if __cpp_lib_stacktrace >= 202011L
            #define FROG_HAS_STACKTRACE 1
            #define FROG_STACKTRACE_STD 1
            #pragma message("std::stacktrace")
        #endif
    #endif

    #if !defined(FROG_STACKTRACE_STD) && __has_include(<execinfo.h>)
        #include <execinfo.h>   // Linux-specific, backtrace, backtrace_symbols

        #define FROG_HAS_STACKTRACE 1
        #pragma message("::backtrace")
    #endif

#endif

#ifndef FROG_HAS_STACKTRACE

    namespace frog {

    void error::init_stacktrace()
    {
        // noop
    }

    } // namespace frog

#else

    #if defined(FROG_STACKTRACE_STD)

        namespace frog {

        void error::init_stacktrace()
        {
            stacktrace = "std::stacktrace " + std::to_string( std::stacktrace::current() );
        }

        } // namespace frog

    #else
        #include "frog/utils/assert.hpp"
        #include "frog/utils/string.hpp"
        #include "frog/utils/string_builder.hpp"

        #include <array>
        #include <string_view>
        #include <cstdlib>
        #include <string>

        namespace frog {

        void error::init_stacktrace()
        {
            auto trace = std::array<void*, 256>{};
            int size = ::backtrace(trace.data(), trace.size());

            auto lines = std::unique_ptr<char*, Free>{};
            lines.reset( ::backtrace_symbols(trace.data(), size) );

            stacktrace.clear();

            for (int i = 0; i < size; i++)
            {
                auto full = std::string_view(lines.get()[i]);

                auto func_name = std::string( frog::between(full, '(', '+') );
                demangle(func_name);

                auto lib = frog::basename( std::string( full.substr(0, full.find('(')) ) );
                auto pos = std::string( frog::between(full, '+', ')') );
                auto ptr = std::string( frog::between(full, '[', ']') );

                stacktrace += frog::make_string(lib, " ( ", func_name, " +", pos, " ) [ ", ptr, " ]\n");
                // LOG(frog::make_string(lib, " | ", func_name, " | ", pos, " | ", ptr));
            }
        }

        } // namespace frog

    #endif
#endif
