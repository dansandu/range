#include "catchorg/catch/catch.hpp"
#include "dansandu/range/concatenate.hpp"
#include "dansandu/range/filter.hpp"
#include "dansandu/range/fold.hpp"
#include "dansandu/range/map.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/repeat.hpp"
#include "dansandu/range/take.hpp"
#include "dansandu/range/zip.hpp"

#include <string>
#include <utility>
#include <vector>

using dansandu::range::concatenate::concatenate;
using dansandu::range::filter::filter;
using dansandu::range::fold::fold;
using dansandu::range::map::map;
using dansandu::range::take::take;
using dansandu::range::zip::zip;
using dansandu::range::pipe::operator|;
using dansandu::range::repeat::repeat;

TEST_CASE("Pipe") {
    auto names = std::vector<const char*>{{"Timmy", "Marie", "Billy", "Eddie", "Kate"}};
    auto ages = std::vector<int>{23, 33, 30, 20, 18};
    auto youngerThan25 = [](auto p) { return p.second < 25; };
    auto folder = [](const auto& s, const auto& n) { return s + n; };

    auto range = names | zip(ages) | filter(youngerThan25) | map([](const auto& p) { return p.first; }) | repeat() |
                 take(9) | concatenate(std::vector<const char*>{{"Andy", "Dany"}}) | fold(std::string{}, folder);

    REQUIRE(range == "TimmyEddieKateTimmyEddieKateTimmyEddieKateAndyDany");
}
