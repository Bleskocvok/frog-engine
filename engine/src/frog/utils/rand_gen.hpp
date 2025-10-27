#pragma once

#include <cstdint>      // uint64_t

namespace frog {

// TODO: Add a quick_rand_gen class with the same interface along with quicker
// and less uniform one.

// TODO: Create my own type that would be „more deterministic“ across different
// platforms and that would would with my „fixed“ type.
// template<typename T>
// using uniform_float = std::uniform_real_distribution<T>;

/// (min, max) Both sides inclusive.
template<typename T>
struct uniform_int
{
    T min_, max_;

    constexpr uniform_int(T min_, T max_) : min_(min_), max_(max_)
    { }

    template<typename Gen>
    constexpr T operator()(Gen& gen)
    {
        constexpr auto GEN_MAX = Gen::max();

        // k could result in 0 due to overflow
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
        if (max_ == GEN_MAX && min_ == T( 0 ))
#pragma GCC diagnostic pop
            return gen();

        T k = max_ - min_ + 1;
        T mod = (GEN_MAX + 1) % k;

        T r;
        do
        {
            r = gen();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
        } while (r > GEN_MAX - mod);
#pragma GCC diagnostic pop

        // This fella be slow.
        return (r % k) + min_;
    }
};

// [min, max) Min inclusive, max exclusive.
template<typename T>
struct uniform_float
{
    T min_, max_;

    uniform_float(T min_, T max_) : min_(min_), max_(max_)
    { }

    // Maybe if type == float/double, else the monstrosity
    // template<typename Gen>
    // T operator()(Gen& gen)
    // {
    //     T k = max_ - min_;
    //     T r = T( gen() ) / T( Gen::max() );
    //     return min_ + k * r;
    // }

    // TODO: Actually, just precompute the table if possible.
    template<typename Gen>
    T operator()(Gen& gen)
    {
        T result = min_;
        T chunk = max_ - min_;

        // // Previous version.
        // T two = T( 2 );
        // T zero = T( 0 );
        // chunk /= two;
        // auto coin = uniform_int<unsigned>(0, 1);
        // for (int i = 0; i < 64 && chunk > zero; i++)
        // {
        //     if (coin(gen) == 0)
        //         result += chunk;
        //     chunk /= two;
        // }

        // Slightly faster.
        T zero = T( 0 );
        T two = T( 2 );
        chunk /= two;
        // auto coin = uniform_int<std::uint64_t>(0, std::uint64_t(-1));
        // std::uint64_t bits = coin(gen);
        auto bits = gen();
        static_assert(Gen::max() == std::uint64_t(-1));

        for (int j = 0; j < 64 && chunk > zero; j++)
        {
            if (bits & 0x1)
                result += chunk;
            chunk /= two;
            bits >>= 1;
        }

        return result;
    }
};

class xorshift32
{
    std::uint32_t state;

public:
    constexpr explicit xorshift32(std::uint32_t seed) : state(seed ? seed : 12345) {}

    constexpr void next()
    {
        std::uint32_t x = state;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        state = x;
    }

    constexpr std::uint32_t operator()()
    {
        next();
        return state;
    }

    static constexpr std::uint32_t max()
    {
        return std::uint32_t(-1);
    }
};

class xorshift64
{
    std::uint64_t state = 0;

public:
    constexpr explicit xorshift64(std::uint64_t seed) : state(seed ? seed : 12345) {}

    constexpr void next()
    {
        std::uint64_t x = state;
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        state = x;
    }

    constexpr std::uint64_t operator()()
    {
        next();
        return state;
    }

    static constexpr std::uint64_t max()
    {
        return std::uint64_t(-1);
    }
};

// template<typename Gen = std::mt19937, typename Int = int, typename Float = float>
template<typename Gen = xorshift64, typename Int = int, typename Float = float>
class rand_gen_t
{
    Gen gen;

public:
    rand_gen_t(std::uint64_t seed) : gen(seed) {}

    Int uniform_int(Int min, Int max)
    {
        return frog::uniform_int<Int>(min, max)(gen);
    }

    Float uniform_float(Float min, Float max)
    {
        return frog::uniform_float<Float>(min, max)(gen);
    }
};

using rand_gen = rand_gen_t<>;

} // namespace frog
