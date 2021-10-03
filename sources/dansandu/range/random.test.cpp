#include "dansandu/range/random.hpp"
#include "catchorg/catch/catch.hpp"
#include "dansandu/range/pipe.hpp"
#include "dansandu/range/take.hpp"

#include <algorithm>
#include <cmath>

using dansandu::range::random::random;
using dansandu::range::take::take;
using dansandu::range::pipe::operator|;

TEST_CASE("Random")
{
    auto generator = std::minstd_rand{};

    SECTION("integral values")
    {
        constexpr auto a = 0;
        constexpr auto b = 99;
        constexpr auto sampleCount = 1000;
        constexpr auto bucketCount = 10;
        constexpr auto possibleValues = b - a + 1;

        REQUIRE(possibleValues % bucketCount == 0);

        constexpr auto bucketWidth = possibleValues / bucketCount;
        constexpr auto bucketTarget = sampleCount / bucketCount;
        constexpr auto tolerance = 0.25;

        auto range = random<int>(generator, a, b) | take(sampleCount);

        int buckets[bucketCount] = {0};
        auto withinBounds = true;
        for (auto sample : range)
        {
            if (sample < a || sample > b)
            {
                withinBounds = false;
                break;
            }
            ++buckets[sample / bucketWidth];
        }

        REQUIRE(withinBounds);

        auto uniformDistribution = true;
        for (auto bucket : buckets)
            if (std::abs(bucketTarget - bucket) / static_cast<double>(bucketTarget) > tolerance)
            {
                uniformDistribution = false;
                break;
            }

        REQUIRE(uniformDistribution);
    }

    SECTION("real values")
    {
        constexpr auto a = 0.0;
        constexpr auto b = 1.0;
        constexpr auto interval = b - a;
        constexpr auto sampleCount = 1000;
        constexpr auto bucketCount = 10;

        constexpr auto bucketWidth = interval / bucketCount;
        constexpr auto bucketTarget = sampleCount / bucketCount;
        constexpr auto tolerance = 0.25;

        auto range = random<double>(generator, a, b) | take(sampleCount);

        int buckets[bucketCount] = {0};
        auto withinBounds = true;
        for (auto sample : range)
        {
            if (sample < a || sample >= b)
            {
                withinBounds = false;
                break;
            }
            auto bucket = 0;
            while (sample > (bucket + 1) * bucketWidth && bucket < bucketCount)
                ++bucket;
            ++buckets[bucket];
        }

        REQUIRE(withinBounds);

        auto uniformDistribution = true;
        for (auto bucket : buckets)
            if (std::abs(bucketTarget - bucket) / static_cast<double>(bucketTarget) > tolerance)
            {
                uniformDistribution = false;
                break;
            }

        REQUIRE(uniformDistribution);
    }
}
