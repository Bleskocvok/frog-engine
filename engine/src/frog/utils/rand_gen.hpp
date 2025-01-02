#pragma once

#include <random>       // mt19937
#include <cstdint>      // uint64_t

namespace frog {

// TODO: Add a quick_rand_gen class with the same interface along with quicker
// and less uniform one.

// TODO: Create my own type that would be „more deterministic“ across different
// platforms and that would would with my „fixed“ type.
template<typename T>
using uniform_float = std::uniform_real_distribution<T>;

template<typename T>
struct uniform_int
{
    T min_, max_;

    uniform_int(T min_, T max_) : min_(min_), max_(max_)
    { }

    template<typename Gen>
    T operator()(Gen& gen)
    {
        constexpr auto GEN_MAX = Gen::max();
        T k = max_ - min_ + 1;
        T mod = (GEN_MAX + 1) % k;

        T r;
        do
        {
            r = gen();

        } while (r > GEN_MAX - mod);

        return (r % k) + min_;
    }
};

template<typename Int = int, typename Float = float>
class rand_gen_t
{
    std::mt19937 gen;

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
