#include "catchorg/catch/catch.hpp"
#include "dansandu/range/integers.hpp"
#include "dansandu/range/pipe.hpp"

#include <vector>

using dansandu::range::integers::integers;
using dansandu::range::pipe::operator|;

TEST_CASE("Integers")
{
    SECTION("natural numbers")
    {
        auto range = integers(0, 1, 5);
        auto expected = std::vector<int>{{0, 1, 2, 3, 4}};
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == expected);
    }

    SECTION("even natural numbers")
    {
        auto range = integers(0, 2, 5);
        auto expected = std::vector<int>{{0, 2, 4, 6, 8}};
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == expected);
    }
}
