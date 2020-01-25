#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/take.hpp"

#include <string_view>
#include <utility>
#include <vector>

using dansandu::range::take::take;
using dansandu::range::pipe::operator|;

TEST_CASE("Take")
{
    std::vector<std::string_view> names = {"Beatrice", "Michael", "Lisa"};

    SECTION("zero elements")
    {
        auto range = names | take(0);

        REQUIRE(range.begin() == range.end());
    }

    SECTION("non-empty subinterval")
    {
        auto range = names | take(2);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Beatrice");

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Michael");

        ++iterator;

        REQUIRE(iterator == end);
    }

    SECTION("all elements")
    {
        auto range = names | take(3);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Beatrice");

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Michael");

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Lisa");

        ++iterator;

        REQUIRE(iterator == end);
    }

    SECTION("overhead elements")
    {
        auto range = names | take(4);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Beatrice");

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Michael");

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Lisa");

        ++iterator;

        REQUIRE(iterator == end);
    }
}
