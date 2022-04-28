#include "dansandu/range/repeat.hpp"
#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/take.hpp"
#include "dansandu/range/to_vector.hpp"

#include <vector>

using dansandu::range::repeat::repeat;
using dansandu::range::take::take;
using dansandu::range::to_vector::toVector;
using dansandu::range::pipe::operator|;

TEST_CASE("repeat")
{
    auto range = repeat(3) | take(5) | toVector();
    auto expected = std::vector<int>{{3, 3, 3, 3, 3}};
    auto actual = std::vector<int>{range.begin(), range.end()};

    REQUIRE(actual == expected);
}
