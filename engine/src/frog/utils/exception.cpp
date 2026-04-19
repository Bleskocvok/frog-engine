#include "exception.hpp"

#include "frog/utils/assert.hpp"
#include "frog/utils/string_builder.hpp"

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

std::string_view between(std::string_view view, char open, char close)
{
    auto start = view.find(open);
    auto end = view.find(close);
    if (start == view.npos)
        return std::string_view{};

    view = view.substr(start, end - start);
    if (not view.empty())
        view.remove_prefix(1);

    return view;
}

} // namespace

namespace frog {

void error::init_stacktrace()
{
#ifdef FROG_HAS_STACKTRACE
    stacktrace = std::to_string( std::stacktrace::current() );
#endif

    auto trace = std::array<void*, 256>{};
    int size = ::backtrace(trace.data(), trace.size());

    auto lines = std::unique_ptr<char*, Free>{};
    lines.reset( ::backtrace_symbols(trace.data(), size) );

    stacktrace.clear();

    for (int i = 0; i < size; i++)
    {
        auto full = std::string_view(lines.get()[i]);

        auto func_name = std::string( between(full, '(', '+') );
        demangle(func_name);

        auto lib = frog::basename( std::string( full.substr(0, full.find('(')) ) );
        auto pos = std::string( between(full, '+', ')') );
        auto ptr = std::string( between(full, '[', ']') );

        stacktrace += frog::make_string(lib, " ( ", func_name, " +", pos, " ) [ ", ptr, " ]\n");
        // LOG(frog::make_string(lib, " | ", func_name, " | ", pos, " | ", ptr));
    }
}

} // namespace frog
