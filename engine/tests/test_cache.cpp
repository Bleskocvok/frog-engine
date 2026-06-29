#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "frog/font/cache.hpp"

TEST_CASE("cache")
{
    using Cache = frog::font::Cache<int, int>;

    auto to_val = [](int i){ return 1000 + i; };

    Cache c( 10 );

    for (int i = 0; i < 10; i++)
    {
        const auto& rv = c.put(i, to_val(i));
        CHECK_EQ(rv, to_val(i));

        for (int j = 0; j <= i; j++)
        {
            CHECK(c.is_present(j));

            auto* ptr = c.get(j);
            CHECK_NE(ptr, nullptr);
            CHECK_EQ(*ptr, to_val(j));
        }
    }

    {
        const auto& rv = c.put(10, to_val(10));
        CHECK_EQ(rv, to_val(10));

        CHECK_FALSE(c.is_present(0));
        auto* ptr = c.get(0);
        CHECK_EQ(ptr, nullptr);
    }

    for (int i = 1; i < 11; i++)
    {
        CHECK(c.is_present(i));

        auto* ptr = c.get(i);
        CHECK_NE(ptr, nullptr);
        CHECK_EQ(*ptr, to_val(i));
    }

    {
        const auto& rv = c.put(11, to_val(11));
        CHECK_EQ(rv, to_val(11));

        CHECK_FALSE(c.is_present(1));
        auto* ptr = c.get(1);
        CHECK_EQ(ptr, nullptr);
    }

    for (int i = 2; i < 12; i++)
    {
        CHECK(c.is_present(i));

        auto* ptr = c.get(i);
        CHECK_NE(ptr, nullptr);
        CHECK_EQ(*ptr, to_val(i));
    }
}
