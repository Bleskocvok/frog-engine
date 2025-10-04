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
        if (max_ == GEN_MAX && min_ == T( 0 ))
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

    template<typename Gen>
    T operator()(Gen& gen)
    {
        T chunk = max_ - min_;
        T two = T( 2 );
        T zero = T( 0 );
        chunk /= two;

        T result = min_;

        // TODO: Generate whole words and then read their bits by bit
        // operations. Perhaps std::bitset?
        auto coin = uniform_int<unsigned>(0, 1);

        for (int i = 0; i < 64 && chunk > zero; i++)
        {
            if (coin(gen) == 0)
                result += chunk;

            chunk /= two;
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
