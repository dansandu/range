#include "catchorg/catch/catch.hpp"
#include "summer_school/range/take.hpp"
#include "summer_school/range/zip.hpp"

#include <string_view>
#include <utility>
#include <vector>

using summer_school::range::take::take;
using summer_school::range::zip::zip;
using summer_school::range::category::operator|;
using std::string_view_literals::operator""sv;

TEST_CASE("Pipe") {
    std::vector<std::string_view> names = {"Timmy", "Marie", "Billy"};
    std::vector<std::string_view> favoritePets = {"cats", "dogs", "fish"};

    using expected_type = std::vector<std::pair<std::string_view, std::string_view>>;
    expected_type expected = {{"Timmy", "cats"}, {"Marie", "dogs"}};

    auto range = names | zip(favoritePets) | take(2);
    auto actual = expected_type{range.begin(), range.end()};

    REQUIRE(actual == expected);
}
