#include "dansandu/range/hash.hpp"
#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/take.hpp"

#include <vector>

using dansandu::range::hash::hash;
using dansandu::range::take::take;
using dansandu::range::pipe::operator|;

TEST_CASE("Hash")
{
    auto integers = std::vector<int>{{3, 17, 57}};
    auto hash1 = integers | take(1) | hash();
    auto hash2 = integers | take(2) | hash();
    auto hash3 = integers | hash();

    REQUIRE(hash1 != hash2);

    REQUIRE(hash1 != hash3);

    REQUIRE(hash2 != hash3);
}
