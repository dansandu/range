#include "catchorg/catch/catch.hpp"
#include "dansandu/range/filter.hpp"
#include "dansandu/range/pipe.hpp"

#include <vector>

using dansandu::range::filter::filter;
using dansandu::range::pipe::operator|;

TEST_CASE("Filter") {
    auto isEven = [](auto i) { return i % 2 == 0; };

    SECTION("non-empty range") {
        auto integers = std::vector<int>{{0, 1, 2, 3, 4}};

        auto range = integers | filter(isEven);
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == std::vector<int>{{0, 2, 4}});
    }

    SECTION("non-empty range") {
        auto range = std::vector<int>{} | filter(isEven);

        REQUIRE(range.begin() == range.end());
    }
}
