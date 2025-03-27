#pragma once

#include "basic.hpp"

#include <utility>      // std::forward
#include <algorithm>    // std::min
#include <string>       // std::to_string
#include <ostream>
#include <sstream>
#include <cmath>
#include <array>


namespace frog::geo
{


template<typename T, unsigned Dim>
class vec;

template<typename T, unsigned Width, unsigned Height>
class matrix;


namespace detail
{


// damn you, MSVC!
// this function is a hack to make it work
template<typename Array>
constexpr int get_size()
{
    return Array{}.size();
}


template<unsigned Idx = 0, typename Func, typename Array, typename Array2>
constexpr void zip(Func func, Array& array, Array2& other)
{
    func(array[Idx], other[Idx]);
    // damn you, MSVC!
    // why can't `std::array::size` be used here?
    // if constexpr (Idx < array.size() - 1)
    if constexpr (Idx < get_size<Array>() - 1)
    {
        zip<Idx + 1>(func, array, other);
    }
}


template<int Idx = 0, typename Func, typename Array>
constexpr void for_each(Func func, Array& array)
{
    func(array[Idx]);
    if constexpr (Idx < get_size<Array>() - 1)
    {
        for_each<Idx + 1>(func, array);
    }
}


template<int Idx = 0, typename Array, typename Arg, typename ... Args>
constexpr void fill_array(Array& array, Arg&& arg, Args&& ... args)
{
    constexpr size_t count = 1 + sizeof...(Args);
    static_assert(get_size<Array>() - Idx == count,
            "'Size - Idx' must be equal to the number of arguments");
    array[Idx] = std::forward<Arg>(arg);
    if constexpr (count >= 2)
    {
        fill_array<Idx + 1>(array, std::forward<Args>(args)...);
    }
}


template<typename Array, typename L, typename R, typename Del>
std::string to_str(const Array& array,
                    const L& l_border,
                    const R& r_border,
                    const Del& delimiter)
{
    std::ostringstream res;
    auto del = "";
    res << l_border;
    for (auto& elem : array)
    {
        res << del << elem;
        del = delimiter;
    }
    res << r_border;
    return res.str();
}

} // namespace detail



template<typename T, unsigned Size, typename Derived>
class element
{
public:
    std::array<T, Size> data = { 0 };

    constexpr element() = default;

    const T* ptr() const
    {
        return &data.front();
    }

    constexpr bool is_close(const Derived& other) const
    {
        bool res = true;
        detail::zip([&](const auto& u, const auto& v)
        {
            res = res && frog::geo::is_close(u, v);
        }, data, other.data);
        return res;
    }

    constexpr friend Derived operator+(const Derived& one, const Derived& other)
    {
        Derived res = one;
        detail::zip([](auto& u, const auto& v)
        {
            u += v;
        }, res.data, other.data);
        return res;
    }

    constexpr friend Derived operator-(const Derived& one, const Derived& other)
    {
        Derived res = one;
        detail::zip([](auto& u, const auto& v)
        {
            u -= v;
        }, res.data, other.data);
        return res;
    }

    constexpr friend Derived operator-(Derived one)
    {
        detail::for_each([](auto& u){ u = -u; }, one.data);
        return one;
    }

    constexpr friend Derived& operator+=(Derived& one, const Derived& other)
    {
        detail::zip([](auto& u, const auto& v)
        {
            u += v;
        }, one.data, other.data);
        return one;
    }

    constexpr friend Derived& operator-=(Derived& one, const Derived& other)
    {
        detail::zip([](auto& u, const auto& v)
        {
            u -= v;
        }, one.data, other.data);
        return one;
    }

    constexpr friend Derived operator*(const Derived& one, T val)
    {
        Derived res = one;
        detail::for_each([=](auto& u){ u *= val; }, res.data);
        return res;
    }

    constexpr friend Derived operator*(T val, const Derived& one)
    {
        return one * val;
    }

    constexpr friend Derived operator/(const Derived& one, T val)
    {
        Derived res = one;
        detail::for_each([=](auto& u){ u /= val; }, res.data);
        return res;
    }

    constexpr friend Derived& operator*=(Derived& one, T val)
    {
        detail::for_each([=](auto& u){ u *= val; }, one.data);
        return one;
    }

    constexpr friend Derived& operator/=(Derived& one, T val)
    {
        detail::for_each([=](auto& u){ u /= val; }, one.data);
        return one;
    }

    constexpr friend bool operator==(const Derived& one, const Derived& other)
    {
        bool res = true;
        detail::zip([&](const auto& u, const auto& v)
        {
            res = res && (u == v);
        }, one.data, other.data);
        return res;
    }

    constexpr friend bool operator!=(const Derived& one, const Derived& other)
    {
        return not (one == other);
    }
};


} // namespace frog::geo
