#include "dansandu/range/fold.hpp"
#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"

#include <string>
#include <vector>

using dansandu::range::fold::fold;
using dansandu::range::pipe::operator|;

TEST_CASE("Fold")
{
    auto add = [](const auto& a, const auto& b) { return a + b; };

    SECTION("integers")
    {
        auto integers = std::vector<int>{{0, 1, 2, 3, 4}};
        auto initialValue = 0;
        auto sum = integers | fold(initialValue, add);

        REQUIRE(sum == 10);
    }

    SECTION("strings")
    {
        auto strings = std::vector<std::string>{{"Ana ", "are ", "mere."}};
        auto initialValue = std::string{};
        auto concatenation = strings | fold(initialValue, add);

        REQUIRE(concatenation == "Ana are mere.");
    }

    SECTION("empty range")
    {
        auto integers = std::vector<int>{};
        auto initialValue = 0;
        auto sum = integers | fold(initialValue, add);

        REQUIRE(sum == initialValue);
    }
}
