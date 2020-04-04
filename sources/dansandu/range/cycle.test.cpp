#include "catchorg/catch/catch.hpp"
#include "dansandu/range/cycle.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/take.hpp"
#include "dansandu/range/to_vector.hpp"

#include <vector>

using dansandu::range::cycle::cycle;
using dansandu::range::pipe::operator|;
using dansandu::range::take::take;
using dansandu::range::to_vector::toVector;

TEST_CASE("Cycle")
{
    SECTION("non-empty range")
    {
        auto integers = std::vector<int>{{1, 2, 3}};
        auto actual = integers | cycle() | take(8) | toVector();
        auto expected = std::vector<int>{{1, 2, 3, 1, 2, 3, 1, 2}};

        REQUIRE(actual == expected);
    }

    SECTION("singleton cycle")
    {
        auto integers = std::vector<int>(1, 3);
        auto actual = integers | cycle() | take(5) | toVector();
        auto expected = std::vector<int>{{3, 3, 3, 3, 3}};

        REQUIRE(actual == expected);
    }

    SECTION("empty range")
    {
        auto integers = std::vector<int>{};
        auto cycled = integers | cycle() | take(8) | toVector();

        REQUIRE(cycled.empty());
    }
}
