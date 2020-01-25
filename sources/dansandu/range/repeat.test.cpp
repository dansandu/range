#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/repeat.hpp"

#include <vector>

using dansandu::range::repeat::repeat;
using dansandu::range::pipe::operator|;

TEST_CASE("Repeat")
{
    auto vector = std::vector<int>{{1, 2}};
    auto range = vector | repeat();
    auto iterator = range.begin();

    REQUIRE(iterator != range.end());

    REQUIRE(*iterator == 1);

    ++iterator;

    REQUIRE(iterator != range.end());

    REQUIRE(*iterator == 2);

    ++iterator;

    REQUIRE(iterator != range.end());

    REQUIRE(*iterator == 1);
}
