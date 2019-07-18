#include "catchorg/catch/catch.hpp"
#include "summer_school/range/zip.hpp"

#include <string_view>
#include <utility>
#include <vector>

using summer_school::range::zip::zip;
using summer_school::range::category::operator|;
using std::string_view_literals::operator""sv;

TEST_CASE("Zip") {
    SECTION("equal ranges") {
        std::vector<std::string_view> names = {"Annie", "John", "Sam"};
        std::vector<int> ages = {23, 32, 48};

        auto range = names | zip(ages);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair("Annie"sv, 23));

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair("John"sv, 32));

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair("Sam"sv, 48));

        ++iterator;

        REQUIRE(iterator == end);
    }

    SECTION("first range smaller than the second") {
        std::vector<std::string_view> names = {"Jim", "Arthas"};
        std::vector<int> ages = {32, 22, 15};

        auto range = names | zip(ages);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair("Jim"sv, 32));

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair("Arthas"sv, 22));

        ++iterator;

        REQUIRE(iterator == end);
    }

    SECTION("second range smaller than the first") {
        std::vector<std::string_view> names = {"Zusan", "John", "Lizbeth"};
        std::vector<int> ages = {50, 60};

        auto range = names | zip(ages);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair("Zusan"sv, 50));

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair("John"sv, 60));

        ++iterator;

        REQUIRE(iterator == end);
    }

    SECTION("lvalue and rvalue ranges") {
        std::vector<std::string_view> names = {"Matt", "Kate"};
        std::vector<std::string_view> favoriteColours = {"magenta", "aquamarine"};

        using expected_type = std::vector<std::pair<std::string_view, std::string_view>>;
        expected_type expected = {{"Matt", "magenta"}, {"Kate", "aquamarine"}};

        SECTION("lvalue on the left and rvalue on the right") {
            auto range = names | zip(std::move(favoriteColours));
            auto actual = expected_type{range.begin(), range.end()};

            REQUIRE(favoriteColours.empty());

            REQUIRE(actual == expected);
        }

        SECTION("rvalue on the left and lvalue on the right") {
            auto range = std::move(names) | zip(favoriteColours);
            auto actual = expected_type{range.begin(), range.end()};

            REQUIRE(names.empty());

            REQUIRE(actual == expected);
        }

        SECTION("rvalue on the left and rvalue on the right") {
            auto range = std::move(names) | zip(std::move(favoriteColours));
            auto actual = expected_type{range.begin(), range.end()};

            REQUIRE(names.empty());

            REQUIRE(favoriteColours.empty());

            REQUIRE(actual == expected);
        }
    }
}
