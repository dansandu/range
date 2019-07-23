#include "catchorg/catch/catch.hpp"
#include "dansandu/range/map.hpp"
#include "dansandu/range/pipe.hpp"

#include <vector>

using dansandu::range::map::map;
using dansandu::range::pipe::operator|;

TEST_CASE("Map") {
    auto square = [](auto i) { return i * i; };
    auto integers = std::vector<int>{{0, 1, 2, 3, 4}};

    auto range = integers | map(square);
    auto actual = std::vector<int>{range.begin(), range.end()};

    REQUIRE(actual == std::vector<int>{{0, 1, 4, 9, 16}});
}
