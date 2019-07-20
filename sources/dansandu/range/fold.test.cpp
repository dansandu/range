#include "catchorg/catch/catch.hpp"
#include "dansandu/range/fold.hpp"
#include "dansandu/range/pipe.hpp"

#include <string>
#include <vector>

using dansandu::range::fold::fold;
using dansandu::range::pipe::operator|;

TEST_CASE("Fold") {
    auto add = [](const auto& a, const auto& b) { return a + b; };

    SECTION("integers") {
        auto integers = std::vector<int>{{0, 1, 2, 3, 4}};
        auto sum = integers | fold(0, add);

        REQUIRE(sum == 10);
    }

    SECTION("strings") {
        auto strings = std::vector<std::string>{{"Ana ", "are ", "mere."}};
        auto concatenation = strings | fold(std::string{}, add);

        REQUIRE(concatenation == "Ana are mere.");
    }

    SECTION("empty range") {
        auto intialVale = 0;
        auto integers = std::vector<int>{};
        auto sum = integers | fold(intialVale, add);

        REQUIRE(sum == intialVale);
    }
}
