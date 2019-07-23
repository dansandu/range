#include "catchorg/catch/catch.hpp"
#include "dansandu/range/filter.hpp"
#include "dansandu/range/pipe.hpp"

#include <cmath>
#include <vector>

using dansandu::range::filter::filter;
using dansandu::range::pipe::operator|;

auto isPrime(int number) {
    auto divisor = 2;
    auto half = (std::abs(number) >> 2);
    while ((divisor < half) & (number % divisor != 0))
        ++divisor;
    return number % divisor != 0;
}

TEST_CASE("Filter") {
    SECTION("with lambda") {
        auto isEven = [](auto i) { return i % 2 == 0; };

        SECTION("non-empty range") {
            auto integers = std::vector<int>{{0, 1, 2, 3, 4}};

            auto range = integers | filter(isEven);
            auto actual = std::vector<int>{range.begin(), range.end()};

            REQUIRE(actual == std::vector<int>{{0, 2, 4}});
        }

        SECTION("empty range") {
            auto range = std::vector<int>{} | filter(isEven);

            REQUIRE(range.begin() == range.end());
        }
    }

    SECTION("with function pointer") {
        SECTION("non-empty range") {
            auto integers = std::vector<int>{{-23, -10, 4, 13, 27, 43}};
            auto range = integers | filter(isPrime);
            auto actual = std::vector<int>{range.begin(), range.end()};

            REQUIRE(actual == std::vector<int>{{-23, 13, 43}});
        }

        SECTION("empty range") {
            auto range = std::vector<int>{} | filter(isPrime);

            REQUIRE(range.begin() == range.end());
        }
    }
}
