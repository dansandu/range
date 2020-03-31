#include "catchorg/catch/catch.hpp"
#include "dansandu/range/integers.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/shuffle.hpp"
#include "dansandu/range/to_vector.hpp"

#include <algorithm>
#include <vector>

using dansandu::range::integers::integers;
using dansandu::range::shuffle::shuffle;
using dansandu::range::to_vector::toVector;
using dansandu::range::pipe::operator|;

TEST_CASE("Shuffle")
{
    SECTION("integers")
    {
        auto myIntegers = integers(0, 1, 100) | toVector();
        auto shuffled = myIntegers | shuffle();

        REQUIRE(shuffled.size() == myIntegers.size());

        REQUIRE(std::is_permutation(shuffled.cbegin(), shuffled.cend(), myIntegers.cbegin()));
    }

    SECTION("empty range")
    {
        auto integers = std::vector<int>{};
        auto shuffled = integers | shuffle();

        REQUIRE(shuffled.empty());
    }
}
