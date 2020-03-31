#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/predictive_random_generator.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/random.hpp"
#include "dansandu/range/take.hpp"

#include <algorithm>
#include <cmath>

using Catch::Detail::Approx;
using dansandu::ballotin::predictive_random_generator::PredictiveRandomGenerator;
using dansandu::range::random::random;
using dansandu::range::take::take;
using dansandu::range::pipe::operator|;

TEST_CASE("Random")
{
    SECTION("integral values")
    {
        SECTION("default parameters")
        {
            auto range = random<int, PredictiveRandomGenerator>() | take(100);

            REQUIRE(std::all_of(range.begin(), range.end(), [](auto i) { return i == 34; }));
        }

        SECTION("non-default parameters")
        {
            auto range = random<int, PredictiveRandomGenerator>(60, 120) | take(100);

            REQUIRE(std::all_of(range.begin(), range.end(), [](auto i) { return i == 110; }));
        }
    }

    SECTION("real values")
    {
        auto epsilon = 1.0e-5;

        SECTION("default parameters")
        {
            auto range = random<double, PredictiveRandomGenerator>() | take(100);

            REQUIRE(std::all_of(range.begin(), range.end(), [epsilon](auto d) { return std::abs(d - 0.5) < epsilon; }));
        }

        SECTION("non-default parameters")
        {
            auto range = random<double, PredictiveRandomGenerator>(5.0, 10.0) | take(100);

            REQUIRE(std::all_of(range.begin(), range.end(), [epsilon](auto d) { return std::abs(d - 7.5) < epsilon; }));
        }
    }
}
