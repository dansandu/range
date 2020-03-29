#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/shuffle.hpp"

#include <algorithm>
#include <string>
#include <type_traits>
#include <vector>

using dansandu::range::shuffle::shuffle;
using dansandu::range::pipe::operator|;

TEST_CASE("Shuffle")
{
    SECTION("integers")
    {
        auto integers = std::vector<int>{{0, 1, 2, 3, 4, 5}};
        auto shuffled = integers | shuffle();

        REQUIRE(shuffled.size() == integers.size());
        REQUIRE(std::is_permutation(integers.cbegin(), integers.cend(), shuffled.cbegin()));
    }

    SECTION("empty range")
    {
        auto integers = std::list<int>{};
        auto shuffled = integers | shuffle();

        REQUIRE(shuffled.empty());
    }
}
