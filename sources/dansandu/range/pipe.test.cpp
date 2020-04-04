#include "catchorg/catch/catch.hpp"
#include "dansandu/range/range.hpp"

#include <string>
#include <utility>
#include <vector>

using namespace dansandu::range::range;

TEST_CASE("Pipe")
{
    auto names = std::vector<const char*>{{"Timmy", "Marie", "Billy", "Eddie", "Kate"}};
    auto ages = std::vector<int>{23, 33, 30, 20, 18};
    auto youngerThan25 = [](auto p) { return p.second < 25; };
    auto folder = [](const auto& s, const auto& n) { return s + n; };

    auto range = names | zip(ages) | filter(youngerThan25) | map(&std::pair<const char*, int>::first) | cycle() |
                 take(9) | concatenate(std::vector<const char*>{{"Andy", "Dany"}}) | fold(std::string{}, folder);

    REQUIRE(range == "TimmyEddieKateTimmyEddieKateTimmyEddieKateAndyDany");
}
