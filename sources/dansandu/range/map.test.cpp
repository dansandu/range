#include "catchorg/catch/catch.hpp"
#include "dansandu/range/map.hpp"
#include "dansandu/range/pipe.hpp"

#include <ostream>
#include <vector>

using dansandu::range::map::map;
using dansandu::range::pipe::operator|;

struct Item {
    explicit Item(int id) : id{id} {}

    auto getId() const { return id; };

    int id;
};

auto scale(int integer) { return 2 * integer; }

TEST_CASE("Map") {
    SECTION("with lambda") {
        auto square = [](auto i) { return i * i; };
        auto range = std::vector<int>{{0, 1, 2, 3, 4}} | map(square);
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == std::vector<int>{{0, 1, 4, 9, 16}});
    }

    SECTION("with function pointer") {
        auto range = std::vector<int>{{0, 1, 2, 3}} | map(scale);
        auto actual = std::vector<int>{range.begin(), range.end()};

        REQUIRE(actual == std::vector<int>{{0, 2, 4, 6}});
    }

    SECTION("with member pointer") {
        auto items = std::vector<Item>{{Item{0}, Item{1}, Item{2}, Item{3}}};

        SECTION("to method") {
            auto range = items | map(&Item::getId);
            auto actual = std::vector<int>{range.begin(), range.end()};

            REQUIRE(actual == std::vector<int>{{0, 1, 2, 3}});
        }

        SECTION("to object") {
            auto range = items | map(&Item::id);
            auto actual = std::vector<int>{range.begin(), range.end()};

            REQUIRE(actual == std::vector<int>{{0, 1, 2, 3}});
        }
    }
}
