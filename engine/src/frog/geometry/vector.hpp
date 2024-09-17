#pragma once

#include "element.hpp"

#include <cassert>


namespace frog::geo
{


template<typename T, int Dim>
class vec;

using vec2 = vec<float, 2>;
using vec3 = vec<float, 3>;
using vec4 = vec<float, 4>;

using ivec2 = vec<int, 2>;
using ivec3 = vec<int, 3>;
using ivec4 = vec<int, 4>;


template<typename T, int Dim>
class vec : public element<T, Dim, vec<T, Dim>>
{
    using Base = element<T, Dim, vec<T, Dim>>;
    friend Base;

public:
    vec() = default;

    template<typename ... Args>
    vec(T t1, T t2, Args&& ... args)
    {
        static_assert(sizeof...(Args) == Dim - 2,
                "Number of arguments must be equal to vector Dimension");
        detail::fill_array(
                Base::data,
                std::forward<T>(t1),
                std::forward<T>(t2),
                std::forward<Args>(args)...);
    }

    vec(vec<T, Dim - 1> v, T&& val)
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

    vec(T id)
    {
        detail::for_each([=](auto& elem){ elem = id; }, Base::data);
    }

    /**
     * Member access
     */

    T& operator[](size_t i)
    {
        assert(i < Base::data.size());
        return Base::data[i];
    }
    const T& operator[](size_t i) const
    {
        assert(i < Base::data.size());
        return Base::data[i];
    }

    const T& x() const { return get<0>(); }
          T& x()       { return get<0>(); }

    const T& y() const { return get<1>(); }
          T& y()       { return get<1>(); }

    const T& z() const { return get<2>(); }
          T& z()       { return get<2>(); }

    const T& w() const { return get<3>(); }
          T& w()       { return get<3>(); }

    const T& r() const { return x(); }
          T& r()       { return x(); }

    const T& g() const { return y(); }
          T& g()       { return y(); }

    const T& b() const { return z(); }
          T& b()       { return z(); }

    const T& a() const { return w(); }
          T& a()       { return w(); }


    void is_color() const
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

    T length() const
    {
        return std::sqrt(length_squared());
    }

    T length_squared() const
    {
        T squared = { 0 };
        detail::for_each([&](auto& el)
        {
            squared += std::pow(el, 2);
        }, Base::data);

        return squared;
    }

    T dot(const vec& other) const
    {
        T sum{};
        detail::zip([&](auto& a, const auto& b)
        {
            sum += a * b;
        }, Base::data, other.data);
        return sum;
    }

    vec cross(const vec& other) const
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

    vec& normalize()
    {
        *this /= length();
        return *this;
    }

    vec normalized() const
    {
        return (*this) / length();
    }

    matrix<T, Dim, 1> to_matrix_h() const
    {
        auto mat = matrix<T, Dim, 1>(0);
        mat.data = this->data;
        return mat;
    }

    matrix<T, 1, Dim> to_matrix_v() const
    {
        auto mat = matrix<T, 1, Dim>(0);
        mat.data = this->data;
        return mat;
    }

    vec& operator*=(vec other)
    {
        detail::zip([](auto& u, const auto& v){ u *= v; }, this->data, other.data);
        return *this;
    }

    friend vec operator*(vec a, vec b)
    {
        a *= b;
        return a;
    }

private:
    // TODO add enable_if
    template<int Idx>
    T& get()
    {
        static_assert(Dim > Idx);
        return Base::data[Idx];
    }

    template<int Idx>
    const T& get() const
    {
        static_assert(Dim > Idx);
        return Base::data[Idx];
    }
};


} // namespace frog::geo
