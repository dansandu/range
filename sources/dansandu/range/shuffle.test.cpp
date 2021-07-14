#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/test/random.hpp"
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
using dansandu::ballotin::test::random::PredictableBitGenerator;

TEST_CASE("Shuffle")
{
    auto generator = PredictableBitGenerator{};

    SECTION("integers")
    {
        const auto myIntegers = integers(0, 1, 10) | toVector();
        const auto shuffled = myIntegers | shuffle(generator);
        const auto expected = std::vector<int>{{0, 2, 4, 6, 8, 5, 7, 9, 3, 1}};

        REQUIRE(expected == shuffled);
    }

    SECTION("empty range")
    {
        const auto integers = std::vector<int>{};
        const auto shuffled = integers | shuffle(generator);

        REQUIRE(shuffled.empty());
    }
}
