#include "dansandu/range/integers.hpp"
#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"

#include <vector>

using dansandu::range::integers::integers;
using dansandu::range::pipe::operator|;

TEST_CASE("integers")
{
    SECTION("natural finite numbers")
    {
        const auto range = integers(0, 1, 5);
        const auto expected = std::vector<int>{{0, 1, 2, 3, 4}};
        const auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == expected);
    }

    SECTION("even natural finite numbers")
    {
        const auto range = integers(0, 2, 5);
        const auto expected = std::vector<int>{{0, 2, 4, 6, 8}};
        const auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == expected);
    }

    SECTION("negative infinite numbers")
    {
        const auto range = integers(-1, -5);
        const auto end = range.end();
        const auto expected = std::vector<int>{{-1, -6, -11, -16, -21}};

        auto iterator = range.begin();
        auto actual = std::vector<int>{};
        for (auto i = 0; i < 5; ++i)
        {
            if (iterator != end)
            {
                actual.push_back(*iterator++);
            }
        }

        REQUIRE(actual == expected);
    }

    SECTION("positive infinite numbers")
    {
        const auto range = integers(5, 2);
        const auto end = range.end();
        const auto expected = std::vector<int>{{5, 7, 9, 11, 13}};

        auto iterator = range.begin();
        auto actual = std::vector<int>{};
        for (auto i = 0; i < 5; ++i)
        {
            if (iterator != end)
            {
                actual.push_back(*iterator++);
            }
        }

        REQUIRE(actual == expected);
    }
}
