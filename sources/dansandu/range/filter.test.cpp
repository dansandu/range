#include <ostream>

struct Box {
    Box(int id, bool isEmpty) : id{id}, isEmpty{isEmpty} {}

    auto empty() const { return isEmpty; };

    int id;
    bool isEmpty;
};

auto operator==(Box a, Box b) { return a.id == b.id && a.isEmpty == b.isEmpty; }

auto operator!=(Box a, Box b) { return !(a == b); }

std::ostream& operator<<(std::ostream& stream, Box box) {
    return stream << "Box(" << box.id << ", " << box.isEmpty << ")";
}

#include "catchorg/catch/catch.hpp"
#include "dansandu/range/filter.hpp"
#include "dansandu/range/pipe.hpp"

#include <cmath>
#include <vector>

using dansandu::range::filter::filter;
using dansandu::range::pipe::operator|;

static auto isPrime(int number) {
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
            auto integers = std::vector<int>{{0, 1, 2, 3, 4, 5}};

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
            auto integers = std::vector<int>{{-10, -23, -10, 4, 13, 27, 43}};
            auto range = integers | filter(isPrime);
            auto actual = std::vector<int>{range.begin(), range.end()};

            REQUIRE(actual == std::vector<int>{{-23, 13, 43}});
        }

        SECTION("empty range") {
            auto range = std::vector<int>{} | filter(isPrime);

            REQUIRE(range.begin() == range.end());
        }
    }

    SECTION("with member pointer") {
        auto boxes = std::vector<Box>{{{0, false}, {1, true}, {2, true}, {3, false}}};
        auto expected = std::vector<Box>{{{1, true}, {2, true}}};

        SECTION("to object") {
            auto range = boxes | filter(&Box::isEmpty);
            auto actual = std::vector<Box>{range.begin(), range.end()};

            REQUIRE(actual == expected);
        }

        SECTION("to method") {
            auto range = boxes | filter(&Box::empty);
            auto actual = std::vector<Box>{range.begin(), range.end()};

            REQUIRE(actual == expected);
        }
    }
}
