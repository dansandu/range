#include "dansandu/range/to_vector.hpp"
#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"

#include <algorithm>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

using dansandu::range::to_vector::toVector;
using dansandu::range::pipe::operator|;

TEST_CASE("ToVector")
{
    SECTION("integers")
    {
        auto integers = std::list<int>{{0, 1, 2, 3, 4}};
        auto vector = integers | toVector();

        REQUIRE(std::equal(integers.cbegin(), integers.cend(), vector.cbegin(), vector.cend()));
        REQUIRE(std::is_same_v<std::vector<int>, decltype(vector)>);
    }

    SECTION("strings")
    {
        auto strings = std::list<std::string>{{"To ", "be ", "continued."}};
        auto vector = strings | toVector();

        REQUIRE(std::equal(strings.cbegin(), strings.cend(), vector.cbegin(), vector.cend()));
        REQUIRE(std::is_same_v<std::vector<std::string>, decltype(vector)>);
    }

    SECTION("empty range")
    {
        auto integers = std::list<int>{};
        auto vector = integers | toVector();

        REQUIRE(std::is_same_v<std::vector<int>, decltype(vector)>);
        REQUIRE(std::equal(integers.cbegin(), integers.cend(), vector.cbegin(), vector.cend()));
    }
}
