#include "catchorg/catch/catch.hpp"
#include "dansandu/range/integers.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/take.hpp"
#include "dansandu/range/zip.hpp"

#include <string_view>
#include <utility>
#include <vector>

using dansandu::range::integers::integers;
using dansandu::range::take::take;
using dansandu::range::zip::zip;
using dansandu::range::pipe::operator|;

TEST_CASE("Pipe") {
    std::vector<std::string_view> names = {"Timmy", "Marie", "Billy"};

    using expected_type = std::vector<std::pair<int, std::string_view>>;
    expected_type expected = {{0, "Timmy"}, {1, "Marie"}};

    auto range = integers() | zip(names) | take(2);
    auto actual = expected_type{range.begin(), range.end()};

    REQUIRE(actual == expected);
}
