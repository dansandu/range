#include "dansandu/range/concatenate.hpp"
#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"

using dansandu::range::concatenate::concatenate;
using dansandu::range::pipe::operator|;

TEST_CASE("concatenate")
{
    SECTION("empty with empty")
    {
        auto even = std::vector<int>{};
        auto odd = std::vector<int>{};

        auto range = even | concatenate(odd);

        REQUIRE(range.begin() == range.end());
    }

    SECTION("non-empty with empty")
    {
        auto even = std::vector<int>{{0, 2, 4}};
        auto odd = std::vector<int>{};

        auto range = even | concatenate(odd);
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == std::vector<int>{{0, 2, 4}});
    }

    SECTION("empty with non-empty")
    {
        auto even = std::vector<int>{};
        auto odd = std::vector<int>{{1, 3, 5}};

        auto range = even | concatenate(odd);
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == std::vector<int>{{1, 3, 5}});
    }

    SECTION("empty with non-empty")
    {
        auto even = std::vector<int>{{0, 2, 4}};
        auto odd = std::vector<int>{{1, 3, 5}};

        auto range = even | concatenate(odd);
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == std::vector<int>{{0, 2, 4, 1, 3, 5}});
    }

    SECTION("second range can be converted to first")
    {
        auto even = std::vector<int>{{0, 2, 4}};
        auto letters = std::vector<char>{{'a', 'b', 'c'}};

        auto range = even | concatenate(letters);
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == std::vector<int>{{0, 2, 4, 97, 98, 99}});
    }
}
