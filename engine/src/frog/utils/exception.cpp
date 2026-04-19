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

#include <execinfo.h>   // Linux-specific, backtrace, backtrace_symbols

#include <cxxabi.h>

#include <array>
#include <memory>
#include <cstdlib>
#include <string>
#include <string_view>

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

} // namespace

namespace frog {

bool demangle(std::string& mangled)
{
    int status = 0;
    auto demangled = std::unique_ptr<char[], Free>{};
    demangled.reset( abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status) );

    static constexpr int OK = 0;
    static constexpr int INVALID_MANGLE = -2;

    if (status != OK)
        return false;

    mangled = std::string(demangled.get());
    return true;
}

void error::init_stacktrace()
{
#ifdef FROG_HAS_STACKTRACE
    stacktrace = std::to_string( std::stacktrace::current() );
#endif

    auto trace = std::array<void*, 256>{};
    int size = ::backtrace(trace.data(), trace.size());

    auto lines = std::unique_ptr<char*, Free>{};
    lines.reset( ::backtrace_symbols(trace.data(), size) );

    for (int i = 0; i < size; i++)
    {
        auto view = std::string_view(lines.get()[i]);

        LOGX(view);

        auto start = view.find('(');
        auto end = view.find('+');
        if (start == view.npos)
            continue;

        view = view.substr(start, end - start);
        if (not view.empty())
            view.remove_prefix(1);

        auto func_name = std::string( view );

        demangle(func_name);

        LOGX(func_name);
    }
}

} // namespace frog
