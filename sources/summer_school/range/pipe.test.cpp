#include "catchorg/catch/catch.hpp"
#include "summer_school/range/integers.hpp"
#include "summer_school/range/take.hpp"
#include "summer_school/range/zip.hpp"

#include <string_view>
#include <utility>
#include <vector>

using summer_school::range::integers::integers;
using summer_school::range::take::take;
using summer_school::range::zip::zip;
using summer_school::range::category::operator|;
using std::string_view_literals::operator""sv;

TEST_CASE("Pipe") {
    std::vector<std::string_view> names = {"Timmy", "Marie", "Billy"};

    using expected_type = std::vector<std::pair<int, std::string_view>>;
    expected_type expected = {{0, "Timmy"}, {1, "Marie"}};

    auto range = integers() | zip(names) | take(2);
    auto actual = expected_type{range.begin(), range.end()};

    REQUIRE(actual == expected);
}
