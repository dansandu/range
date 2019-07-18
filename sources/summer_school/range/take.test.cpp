#include "catchorg/catch/catch.hpp"
#include "summer_school/range/take.hpp"

#include <string_view>
#include <utility>
#include <vector>

using summer_school::range::take::take;
using summer_school::range::category::operator|;
using std::string_view_literals::operator""sv;

TEST_CASE("Take") {
    std::vector<std::string_view> names = {"Beatrice", "Michael", "Lisa"};

    SECTION("zero elements") {
        auto range = names | take(0);

        REQUIRE(range.begin() == range.end());
    }

    SECTION("non-empty subinterval") {
        auto range = names | take(2);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Beatrice"sv);

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Michael"sv);

        ++iterator;

        REQUIRE(iterator == end);
    }

    SECTION("all elements") {
        auto range = names | take(3);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Beatrice"sv);

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Michael"sv);

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Lisa"sv);

        ++iterator;

        REQUIRE(iterator == end);
    }

    SECTION("overhead elements") {
        auto range = names | take(4);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Beatrice"sv);

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Michael"sv);

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == "Lisa"sv);

        ++iterator;

        REQUIRE(iterator == end);
    }
}