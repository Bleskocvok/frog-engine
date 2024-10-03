#pragma once

#include <sstream>      // ostringstream
#include <utility>      // move


namespace frog {


template<typename... Args>
std::string make_string(Args&&... args);


class str_builder {
    std::ostringstream data;

public:
    operator std::string() const
    {
        return data.str();
    }

    template<typename T>
    friend str_builder& operator<<(str_builder& o, const T& val)
    {
        o.data << val;
        return o;
    }

    template<typename T>
    friend str_builder&& operator<<(str_builder&& o, const T& val)
    {
        o.data << val;
        return std::move(o);
    }
};


template<typename... Args>
std::string make_string(Args&&... args)
{
    str_builder builder;
    ( builder << ... << args );
    return builder;
}


} // namespace frog
