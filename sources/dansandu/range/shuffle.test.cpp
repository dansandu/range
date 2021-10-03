#include "dansandu/range/shuffle.hpp"
#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/random.hpp"
#include "dansandu/range/integers.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/to_vector.hpp"

#include <algorithm>
#include <vector>

using dansandu::range::integers::integers;
using dansandu::range::shuffle::shuffle;
using dansandu::range::to_vector::toVector;
using dansandu::range::pipe::operator|;
using dansandu::ballotin::random::PredictableBitGenerator;

TEST_CASE("Shuffle")
{
    auto generator = PredictableBitGenerator{};

    SECTION("empty range")
    {
        const auto integers = std::vector<int>{};
        const auto shuffled = integers | shuffle(generator);

        REQUIRE(shuffled.empty());
    }
}
