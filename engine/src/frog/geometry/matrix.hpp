#pragma once

#include "element.hpp"

#include <cassert>
#include <vector>       // vector
#include <algorithm>    // max_element
#include <utility>      // pair


namespace frog::geo
{

template<typename T, unsigned Width, unsigned Height>
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


template<typename T, unsigned Width, unsigned Height>
class matrix : public element<T, Width * Height, matrix<T, Width, Height>>
{
    using Base = element<T, Width * Height, matrix<T, Width, Height>>;
    friend Base;

public:
    static constexpr unsigned W = Width;
    static constexpr unsigned H = Height;

    constexpr matrix(T id)
    {
        for (unsigned i = 0; i < std::min(Width, Height); i++)
        {
            at(i, i) = id;
        }
    }

    template<typename ... Args>
    constexpr matrix(T t, Args&& ... args)
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

    constexpr const T& at(int x, int y) const
    {
        assert(x >= 0 && y >= 0 && x < int(Width) && y < int(Height));
        return Base::data[y * Width + x];
    }
    constexpr T& at(int x, int y)
    {
        assert(x >= 0 && y >= 0 && x < int(Width) && y < int(Height));
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
        for (unsigned y = 0; y < Height; y++)
        {
            res << del;
            del = ", ";
            auto row = std::vector<T>(Width);
            int i = 0;
            std::generate(row.begin(), row.end(), [=, this, &i]()
            {
                return at(i++, y);
            });
            res << detail::to_str(row, "[ ", " ]", ", ");
        }
        res << " ]";
        return res.str();
    }

    constexpr friend std::ostream& operator<<(std::ostream& out, matrix mat)
    {
        return (out << mat.to_str());
    }

    /**
     * Matrix specific arithmetics
     */

    template<unsigned Dim>
    friend constexpr auto operator*(const vec<T, Dim>& v, const matrix& mat)
    {
        static_assert(Dim == Height, "Required vector dimension");
        return mat.transposed() * v;
    }

    template<unsigned Dim>
    friend constexpr auto operator*(const matrix& mat, const vec<T, Dim>& v)
    {
        static_assert(Width == Dim, "Required vector dimension");
        vec<T, Height> result;
        auto mul = mat * v.to_matrix_v();
        result.data = mul.data;
        return result;
    }

    template<unsigned W2, unsigned H2>
    friend constexpr auto operator*(const matrix mat,
                                    const matrix<T, W2, H2>& other)
    {
        static_assert(Width == H2,
                "Required dimensions for matrix multiplication");
        constexpr auto I = Width;

        matrix<T, W2, Height> res(0);  // result with correct dimensions
        for (unsigned y = 0; y < res.H; y++)
        {
            for (unsigned x = 0; x < res.W; x++)
            {
                T sum{};
                for (unsigned i = 0; i < I; i++)
                {
                    sum += mat.at(i, y) * other.at(x, i);
                }
                res.at(x, y) = sum;
            }
        }
        return res;
    }

    template<unsigned W2, unsigned H2>
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
        for (unsigned y = 0; y < Height; y++)
        {
            for (unsigned x = 0; x < Width; x++)
            {
                res.at(y, x) = at(x, y);
            }
        }
        return res;
    }

    constexpr matrix<T, Width - 1, Height - 1> submatrix(int div_x) const
    {
        auto result = matrix<T, Width - 1, Height - 1>{ 0 };
        for (unsigned y = 1; y < Height; y++)
        {
            for (unsigned x = 0; x < Width; x++)
            {
                if (int(x) == div_x)
                    continue;
                if (int(x) > div_x)
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
            for (unsigned i = 0; i < Width; i++)
            {
                auto sub = submatrix(i);
                result += coeff * at(i, 0) * sub.det();
                coeff = -coeff;
            }
            return result;
        }
    }

    // TODO: Make this constexpr for god's sake.
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
            using std::vector, std::pair;
            // TODO: Omg, find a way to make this constexpr.
            // Array complains about wrong number of initializers.
            static const auto coords
                = vector<vector<pair<int, int>>>
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
            unsigned idx = 0;
            for (const auto& row : coords)
            {
                auto cell = matrix<T, 2, 2>{ 0 };
                unsigned c = 0;
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
            return gauss_elimination().second;
        }
    }

    constexpr std::pair<matrix, matrix> gauss_elimination() const
    {
        static_assert(Width == Height, "Gaussian elimination is currently "
                                       "implemented only for square matrices.");

        auto swap_rows = [](auto& m, auto i, auto j)
        {
            for (size_t k = 0; k < Width; ++k)
                std::swap(m.at(k, i), m.at(k, j));
        };

        auto add_row_multiple = [](auto& m, auto r, T mult, auto dst)
        {
            for (size_t c = 0; c < Width; ++c)
                m.at(c, dst) += mult * m.at(c, r);
        };

        auto div_row = [](auto& m, auto r, T d)
        {
            for (size_t c = 0; c < Width; ++c)
                m.at(c, r) /= d;
        };

        using Idx = unsigned;

        matrix mat = *this;
        matrix ide = matrix{ 1 };

        auto argmax = [&mat](auto h, auto k)
        {
            auto t_abs = [](T x){ return x < 0 ? -x : x; };
            auto get = [&](Idx i){ return t_abs(mat.at(k, i)); };

            Idx i_max = h;
            for (Idx i = h; i < Height; ++i)
                if (get(i) > get(i_max))
                    i_max = i;
            return i_max;
        };

        auto m = Height;
        auto n = Width;
        Idx h = 0;
        Idx k = 0;

        while (h < m && k < n)
        {
            T i_max = argmax(h, k);
            if (mat.at(k, i_max) == 0)
            {
                ++k;
            }
            else
            {
                swap_rows(mat, h, i_max);
                swap_rows(ide, h, i_max);
                for (Idx i = h + 1; i < m; ++i)
                {
                    auto f = mat.at(k, i) / mat.at(k, h);
                    add_row_multiple(mat, h, -f, i);
                    add_row_multiple(ide, h, -f, i);
                    mat.at(k, i) = 0;
                }
                ++h;
                ++k;
            }
        }

        for (Idx i = 0; i < Height; ++i)
        {
            Idx row = Height - 1 - i;
            Idx col = Width - 1 - i;
            if (mat.at(col, row) == 0)
                continue;
            for (Idx j = 0; j < row; ++j)
            {
                T coef = -mat.at(col, j) / mat.at(col, row);
                add_row_multiple(mat, row, coef, j);
                add_row_multiple(ide, row, coef, j);
            }
        }
        for (Idx i = 0; i < Height; ++i)
        {
            if (mat.at(i, i) != 0 && ide.at(i, i) != 0)
            {
                div_row(ide, i, mat.at(i, i));
                mat.at(i, i) = 1;
            }
        }

        return { mat, ide };
    }
};


} // namespace frog::geo
