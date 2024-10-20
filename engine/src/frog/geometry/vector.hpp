#pragma once

#include "element.hpp"

#include <cassert>


namespace frog::geo
{


template<typename T, unsigned Dim>
class vec;

using vec2 = vec<float, 2>;
using vec3 = vec<float, 3>;
using vec4 = vec<float, 4>;

using ivec2 = vec<int, 2>;
using ivec3 = vec<int, 3>;
using ivec4 = vec<int, 4>;


template<typename T, unsigned Dim>
class vec : public element<T, Dim, vec<T, Dim>>
{
    using Base = element<T, Dim, vec<T, Dim>>;
    friend Base;

public:
    constexpr vec() = default;

    template<typename ... Args>
    constexpr vec(T t1, T t2, Args&& ... args)
    {
        static_assert(sizeof...(Args) == Dim - 2,
                "Number of arguments must be equal to vector Dimension");
        detail::fill_array(
                Base::data,
                std::forward<T>(t1),
                std::forward<T>(t2),
                std::forward<Args>(args)...);
    }

    constexpr vec(vec<T, Dim - 1> v, T&& val)
    {
        for (size_t i = 0; i < Dim - 1; i++)
        {
            Base::data[i] = v.data[i];
        }
        Base::data[Dim - 1] = std::forward<T>(val);
    }

    /**
     * Vector initialization with the same values:
     *   <k; k>, <k; k; k>, etc.
     */

    constexpr vec(T id)
    {
        detail::for_each([=](auto& elem){ elem = id; }, Base::data);
    }

    /**
     * Member access
     */

    constexpr T& operator[](size_t i)
    {
        assert(i < Base::data.size());
        return Base::data[i];
    }
    constexpr const T& operator[](size_t i) const
    {
        assert(i < Base::data.size());
        return Base::data[i];
    }

    constexpr const T& x() const { return get<0>(); }
    constexpr       T& x()       { return get<0>(); }

    constexpr const T& y() const { return get<1>(); }
    constexpr       T& y()       { return get<1>(); }

    constexpr const T& z() const { return get<2>(); }
    constexpr       T& z()       { return get<2>(); }

    constexpr const T& w() const { return get<3>(); }
    constexpr       T& w()       { return get<3>(); }

    constexpr const T& r() const { return x(); }
    constexpr       T& r()       { return x(); }

    constexpr const T& g() const { return y(); }
    constexpr       T& g()       { return y(); }

    constexpr const T& b() const { return z(); }
    constexpr       T& b()       { return z(); }

    constexpr const T& a() const { return w(); }
    constexpr       T& a()       { return w(); }


    constexpr void is_color() const
    {
        static_assert(Dim == 3 || Dim == 4,
                      "Correct dimensions for color vector");
    }

    /**
     * Pretty printing
     */

    std::string to_str() const
    {
        return detail::to_str(Base::data, "< ", " >", "; ");
    }

    friend std::ostream& operator<<(std::ostream& out, vec v)
    {
        return (out << v.to_str());
    }

    /**
     * Vector specific functions
     */

    constexpr T length() const
    {
        return std::sqrt(length_squared());
    }

    constexpr T length_squared() const
    {
        T squared = { 0 };
        detail::for_each([&](auto& el)
        {
            squared += std::pow(el, 2);
        }, Base::data);

        return squared;
    }

    constexpr T dot(const vec& other) const
    {
        T sum{};
        detail::zip([&](auto& a, const auto& b)
        {
            sum += a * b;
        }, Base::data, other.data);
        return sum;
    }

    constexpr vec cross(const vec& other) const
    {
        static_assert(Dim == 3,
                "Cross product is only possible for 3D vectors");
        const auto& u = *this;
        const auto& v = other;
        auto x = u.y() * v.z() - u.z() * v.y();
        auto y = u.z() * v.x() - u.x() * v.z();
        auto z = u.x() * v.y() - u.y() * v.x();
        return vec<T, 3>(x, y, z);
    }

    constexpr vec& normalize()
    {
        *this /= length();
        return *this;
    }

    constexpr vec normalized() const
    {
        return (*this) / length();
    }

    constexpr matrix<T, Dim, 1> to_matrix_h() const
    {
        auto mat = matrix<T, Dim, 1>(0);
        mat.data = this->data;
        return mat;
    }

    constexpr matrix<T, 1, Dim> to_matrix_v() const
    {
        auto mat = matrix<T, 1, Dim>(0);
        mat.data = this->data;
        return mat;
    }

    constexpr vec& operator*=(vec other)
    {
        detail::zip([](auto& u, const auto& v){ u *= v; }, this->data, other.data);
        return *this;
    }

    constexpr friend vec operator*(vec a, vec b)
    {
        a *= b;
        return a;
    }

private:
    // TODO add enable_if
    template<int Idx>
    constexpr T& get()
    {
        static_assert(Dim > Idx);
        return Base::data[Idx];
    }

    template<int Idx>
    constexpr const T& get() const
    {
        static_assert(Dim > Idx);
        return Base::data[Idx];
    }
};


} // namespace frog::geo
