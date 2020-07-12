#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/zip.hpp"

#include <string>
#include <string_view>
#include <utility>
#include <vector>

using dansandu::range::zip::zip;
using dansandu::range::pipe::operator|;

struct Word
{
};

TEST_CASE("Zip")
{
    SECTION("equal ranges")
    {
        std::vector<std::string_view> names = {"Annie", "John", "Sam"};
        std::vector<int> ages = {23, 32, 48};

        auto range = names | zip(ages);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair(std::string_view{"Annie"}, 23));

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair(std::string_view{"John"}, 32));

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair(std::string_view{"Sam"}, 48));

        ++iterator;

        REQUIRE(iterator == end);
    }

    SECTION("first range smaller than the second")
    {
        std::vector<std::string_view> names = {"Jim", "Arthas"};
        std::vector<int> ages = {32, 22, 15};

        auto range = names | zip(ages);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair(std::string_view{"Jim"}, 32));

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair(std::string_view{"Arthas"}, 22));

        ++iterator;

        REQUIRE(iterator == end);
    }

    SECTION("second range smaller than the first")
    {
        std::vector<std::string_view> names = {"Zusan", "John", "Lizbeth"};
        std::vector<int> ages = {50, 60};

        auto range = names | zip(ages);
        auto iterator = range.begin();
        auto end = range.end();

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair(std::string_view{"Zusan"}, 50));

        ++iterator;

        REQUIRE(iterator != end);

        REQUIRE(*iterator == std::make_pair(std::string_view{"John"}, 60));

        ++iterator;

        REQUIRE(iterator == end);
    }

    SECTION("lvalue and rvalue ranges")
    {
        std::vector<std::string_view> names = {"Matt", "Kate"};
        std::vector<std::string_view> favoriteColours = {"magenta", "aquamarine"};

        using expected_type = std::vector<std::pair<std::string_view, std::string_view>>;
        expected_type expected = {{"Matt", "magenta"}, {"Kate", "aquamarine"}};

        SECTION("lvalue on the left and rvalue on the right")
        {
            auto range = names | zip(std::move(favoriteColours));
            auto actual = expected_type{range.begin(), range.end()};

            REQUIRE(favoriteColours.empty());

            REQUIRE(actual == expected);
        }

        SECTION("rvalue on the left and lvalue on the right")
        {
            auto range = std::move(names) | zip(favoriteColours);
            auto actual = expected_type{range.begin(), range.end()};

            REQUIRE(names.empty());

            REQUIRE(actual == expected);
        }

        SECTION("rvalue on the left and rvalue on the right")
        {
            auto range = std::move(names) | zip(std::move(favoriteColours));
            auto actual = expected_type{range.begin(), range.end()};

            REQUIRE(names.empty());

            REQUIRE(favoriteColours.empty());

            REQUIRE(actual == expected);
        }
    }

    SECTION("custom zipper")
    {
        using integers = std::vector<int>;

        SECTION("lambda expression")
        {
            auto zipper = [](auto a, auto b) { return a * b; };
            auto primes = integers{{3, 5, 7, 11}};
            auto morePrimes = integers{{13, 17, 19, 23}};
            auto expected = integers{{39, 85, 133, 253}};

            auto range = primes | zip(zipper, morePrimes);
            auto actual = integers(range.begin(), range.end());

            REQUIRE(actual == expected);
        }

        SECTION("member function pointer")
        {
            using strings = std::vector<std::wstring>;

            auto zipper = static_cast<int (std::wstring::*)(const std::wstring&) const>(&std::wstring::compare);

            auto english = strings{{L"cat", L"food", L"house", L"duck"}};
            auto romanian = strings{{L"pisică", L"mâncare", L"casă", L"rață"}};
            auto expected = integers{{-1, -1, 1, -1}};

            auto range = english | zip(zipper, romanian);
            auto actual = integers(range.begin(), range.end());

            REQUIRE(actual == expected);
        }
    }
}
