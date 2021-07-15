#pragma once

#include "element.hpp"

#include <cassert>


namespace frog::geo
{

template<typename T, int Width, int Height>
class matrix;

using mat2 = matrix<float, 2, 2>;
using mat3 = matrix<float, 3, 3>;
using mat4 = matrix<float, 4, 4>;

using mat23 = matrix<float, 2, 3>;
using mat32 = matrix<float, 3, 2>;

using mat43 = matrix<float, 4, 3>;
using mat34 = matrix<float, 3, 4>;
using mat42 = matrix<float, 4, 2>;
using mat24 = matrix<float, 2, 4>;

using imat2 = matrix<int, 2, 2>;
using imat3 = matrix<int, 3, 3>;
using imat4 = matrix<int, 4, 4>;

using imat23 = matrix<int, 2, 3>;
using imat32 = matrix<int, 3, 2>;

using imat43 = matrix<int, 4, 3>;
using imat34 = matrix<int, 3, 4>;
using imat42 = matrix<int, 4, 2>;
using imat24 = matrix<int, 2, 4>;


template<typename T, int Width, int Height>
class matrix : public element<T, Width * Height, matrix<T, Width, Height>>
{
    using Base = element<T, Width * Height, matrix<T, Width, Height>>;
    friend Base;

public:
    static constexpr int W = Width;
    static constexpr int H = Height;

    matrix(T id)
    {
        for (int i = 0; i < std::min(Width, Height); i++)
        {
            at(i, i) = id;
        }
    }

    template<typename ... Args>
    matrix(T t, Args&& ... args)
    {
        static_assert(sizeof...(Args) == (Width * Height) - 1,
                "Number of arguments must be equal to matrix 'Width * Height'");
        detail::fill_array(Base::data,
                           std::forward<T>(t),
                           std::forward<Args>(args)...);
    }

    /**
     * Member access
     */

    const T& at(int x, int y) const
    {
        assert(x >= 0 && y >= 0 && x < Width && y < Height);
        return Base::data[y * Width + x];
    }
    T& at(int x, int y)
    {
        assert(x >= 0 && y >= 0 && x < Width && y < Height);
        return Base::data[y * Width + x];
    }

    /**
     * Pretty printing
     */

    std::string to_str() const
    {
        std::ostringstream res;
        auto del = "";
        res << "[ ";
        for (int y = 0; y < Height; y++)
        {
            res << del;
            del = ", ";
            auto row = std::vector<T>(Width);
            int i = 0;
            std::generate(row.begin(), row.end(), [=, &i]()
            {
                return at(i++, y);
            });
            res << detail::to_str(row, "[ ", " ]", ", ");
        }
        res << " ]";
        return res.str();
    }

    friend std::ostream& operator<<(std::ostream& out, matrix mat)
    {
        return (out << mat.to_str());
    }

    /**
     * Matrix specific arithmetics
     */

    template<int Dim>
    friend constexpr auto operator*(const vec<T, Dim>& v, const matrix& mat)
    {
        static_assert(Dim == Height, "Required vector dimension");
        return mat.transposed() * v;
    }

    template<int Dim>
    friend constexpr auto operator*(const matrix& mat, const vec<T, Dim>& v)
    {
        static_assert(Width == Dim, "Required vector dimension");
        vec<T, Height> result;
        auto mul = mat * v.to_matrix_v();
        result.data = mul.data;
        return result;
    }

    template<int W2, int H2>
    friend constexpr auto operator*(const matrix mat,
                                    const matrix<T, W2, H2>& other)
    {
        static_assert(Width == H2,
                "Required dimensions for matrix multiplication");
        constexpr auto I = Width;

        matrix<T, W2, Height> res(0);  // result with correct dimensions
        for (int y = 0; y < res.H; y++)
        {
            for (int x = 0; x < res.W; x++)
            {
                T sum{};
                for (int i = 0; i < I; i++)
                {
                    sum += mat.at(i, y) * other.at(x, i);
                }
                res.at(x, y) = sum;
            }
        }
        return res;
    }

    template<int W2, int H2>
    friend constexpr matrix& operator*=(matrix& mat,
                                        const matrix<T, W2, H2>& other)
    {
        mat = mat * other;
        return mat;
    }

    /**
     * Matrix specific functions
     */

    constexpr auto transposed() const
    {
        auto res = matrix<T, Height, Width>(0);
        for (int y = 0; y < Height; y++)
        {
            for (int x = 0; x < Width; x++)
            {
                res.at(y, x) = at(x, y);
            }
        }
        return res;
    }

    constexpr matrix<T, Width - 1, Height - 1> submatrix(int div_x) const
    {
        auto result = matrix<T, Width - 1, Height - 1>{ 0 };
        for (int y = 1; y < Height; y++)
        {
            for (int x = 0; x < Width; x++)
            {
                if (x == div_x)
                    continue;
                if (x > div_x)
                    result.at(x - 1, y - 1) = at(x, y);
                else
                    result.at(x, y - 1) = at(x, y);
            }
        }
        return result;
    }

    constexpr T det() const
    {
        static_assert(Width == Height,
                "Matrix determinant only possible for square matrices");
        constexpr int Size = Width;

        if constexpr (Size <= 0)
        {
            return 1;
        }
        else if constexpr (Size == 1)
        {
            return at(0, 0);
        }
        else
        {
            T result = 0;
            T coeff = 1;
            for (int i = 0; i < Width; i++)
            {
                auto sub = submatrix(i);
                result += coeff * at(i, 0) * sub.det();
                coeff = -coeff;
            }
            return result;
        }
    }

    matrix inverted() const
    {
        static_assert(Width == Height,
                "Matrix inversion only possible for square matrices");
        constexpr int Size = Width;

        assert(det() != T(0));

        if constexpr (Size == 2)
        {
            auto a = at(0, 0);
            auto b = at(1, 0);
            auto c = at(0, 1);
            auto d = at(1, 1);
            return (T(1) / det()) * matrix{ d, -b, -c, a };
        }
        else if constexpr (Size == 3)
        {
            using std::initializer_list, std::pair;
            static const auto coords
                = initializer_list<initializer_list<pair<int, int>>>
            {
                { { 2, 2 }, { 2, 3 }, { 3, 2 }, { 3, 3 } },
                { { 1, 3 }, { 1, 2 }, { 3, 3 }, { 3, 2 } },
                { { 1, 2 }, { 1, 3 }, { 2, 2 }, { 2, 3 } },
                { { 2, 3 }, { 2, 1 }, { 3, 3 }, { 3, 1 } },
                { { 1, 1 }, { 1, 3 }, { 3, 1 }, { 3, 3 } },
                { { 1, 3 }, { 1, 1 }, { 2, 3 }, { 2, 1 } },
                { { 2, 1 }, { 2, 2 }, { 3, 1 }, { 3, 2 } },
                { { 1, 2 }, { 1, 1 }, { 3, 2 }, { 3, 1 } },
                { { 1, 1 }, { 1, 2 }, { 2, 1 }, { 2, 2 } },
            };

            auto result = matrix<T, 3, 3>{ 0 };
            int idx = 0;
            for (const auto& row : coords)
            {
                auto cell = matrix<T, 2, 2>{ 0 };
                int c = 0;
                for (auto[x, y] : row)
                {
                    cell.at(c % 2, c / 2) = at(y - 1, x - 1);
                    c++; // haha
                }
                result.at(idx % Width, idx / Width) = cell.det();
                idx++;
            }
            return (T(1) / det()) * result;
        }
        else
        {
            // TODO
            assert(false); // Not implemented
            return *this;
        }
    }
};


} // namespace frog::geo
