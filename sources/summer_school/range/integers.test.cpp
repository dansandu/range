#include "catchorg/catch/catch.hpp"
#include "summer_school/range/integers.hpp"

#include <vector>

using summer_school::range::integers::integers;
using summer_school::range::category::operator|;

TEST_CASE("Integers") {
    SECTION("natural numbers") {
        auto range = integers(0, 1, 5);
        auto expected = std::vector<int>{{0, 1, 2, 3, 4}};
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == expected);
    }

    SECTION("even natural numbers") {
        auto range = integers(0, 2, 5);
        auto expected = std::vector<int>{{0, 2, 4, 6, 8}};
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == expected);
    }
}
