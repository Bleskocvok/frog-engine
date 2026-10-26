#pragma once

#include "frog/utils/exception.hpp"
#include "frog/utils/string.hpp"
#include "frog/os/timer.hpp"

#include <cstdint>
#include <map>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>      // move, pair
#include <vector>

#define FROG_PROFILE_FUNC() auto guard = ::frog::ProfilerGuard()

namespace frog {

namespace detail {

class ProfilerStack
{
public:
    void push(std::string id)
    {
        data.push_back(std::move(id));
    }

    void pop()
    {
        data.pop_back();
    }

    bool empty() const
    {
        return data.empty();
    }

    const auto& top() const
    {
        if (empty())
            throw error("ProfilerStack::top(): empty");

        return data.back();
    }

private:
    std::vector<std::string> data;
};

} // namespace detail


using ProfilerKey = std::pair<std::string, std::optional<std::string>>;

struct ProfilerItem
{
    std::uint64_t sum = 0;
    unsigned count = 0;

    ProfilerItem() = default;

    double avg() const
    {
        if (count == 0)
            return 0;

        return double(sum) / count;
    }

    void put(std::uint64_t t)
    {
        sum += t;
        count++;
    }

    friend ProfilerItem& operator+=(ProfilerItem& a, const ProfilerItem& b)
    {
        a.sum += b.sum;
        a.count += b.count;
        return a;
    }
};

#ifndef FROG_PROFILE

class ProfilerGuard
{
public:
    ProfilerGuard(const std::string&)
    { }

    ProfilerGuard(const std::source_location& = std::source_location::current())
    { }

    ProfilerGuard(const ProfilerGuard&) = delete;
    ProfilerGuard& operator=(const ProfilerGuard&) = delete;

    static void reset() { }

    static const auto& times()
    {
        static std::map<ProfilerKey, ProfilerItem> times_us;
        return times_us;
    }

    std::optional<std::string> parent() const { return {}; }
};

#else

class ProfilerGuard
{
public:
    ProfilerGuard(std::string name)
        : name(std::move(name))
    {
        init();
    }

    ProfilerGuard(const std::source_location& location = std::source_location::current())
        : name( strip( location.function_name() ) )
    {
        init();
    }

    ProfilerGuard(const ProfilerGuard&) = delete;
    ProfilerGuard& operator=(const ProfilerGuard&) = delete;

    ~ProfilerGuard()
    {
        times_us[ProfilerKey{ name, parent_ }].put( timer.duration_us() );

        stack.pop();
    }

    static void reset()
    {
        times_us.clear();
    }

    static const auto& times()
    {
        return times_us;
    }

    std::optional<std::string> parent() const
    {
        return parent_;
    }

private:
    static inline std::map<ProfilerKey, ProfilerItem> times_us;
    static inline detail::ProfilerStack stack;

    std::string name;

    using Timer = frog::os::timer;

    Timer timer;

    std::optional<std::string> parent_;

    void init()
    {
        timer.reset();

        if (not stack.empty())
            parent_ = stack.top();

        stack.push(name);
    }

    static std::string strip(const char* str)
    {
        auto ooga = frog::between(std::string_view(str), ' ', '(');
        auto last = ooga.find_last_of(':');
        auto sub = ooga.substr(last);
        if (last != std::string_view::npos)
            sub.remove_prefix(1);
        return std::string( sub );

        // return std::string( frog::between(std::string_view(str), ' ', '(') );
    }
};

#endif

} // namespace frog
