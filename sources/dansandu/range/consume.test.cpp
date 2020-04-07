#include "catchorg/catch/catch.hpp"
#include "dansandu/range/consume.hpp"
#include "dansandu/range/pipe.hpp"

#include <vector>

using dansandu::range::consume::consume;
using dansandu::range::pipe::operator|;

TEST_CASE("Consume")
{
    SECTION("non-empty range")
    {
        auto integers = std::vector<int>{{0, 1, 2, 3, 4}};
        auto copy = std::vector<int>{};
        integers | consume([&copy](auto i) { copy.push_back(i); });

        REQUIRE(integers == copy);
    }

    SECTION("empty range")
    {
        auto integers = std::vector<int>{};
        auto copy = std::vector<int>{};
        integers | consume([&copy](auto i) { copy.push_back(i); });

        REQUIRE(integers == copy);
    }
}
