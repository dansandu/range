#include "catchorg/catch/catch.hpp"
#include "dansandu/range/random.hpp"

#include <cmath>

TEST_CASE("Random")
{
    constexpr auto lowerBoundry = 1;
    constexpr auto upperBoundry = 100;
    constexpr auto bucketSize = 20;
    constexpr auto numberOfSamples = 300;
    constexpr auto tolerance = 0.5;
    constexpr auto numberOfBuckets = (upperBoundry - lowerBoundry + 1) / bucketSize;
    constexpr auto idealSamplesPerBucket = numberOfSamples / numberOfBuckets;

    auto range = dansandu::range::random::random(lowerBoundry, upperBoundry);
    auto iterator = range.begin();

    int buckets[numberOfBuckets] = {0};
    auto generatedNumbersAreWithinBoundries = true;
    for (auto i = 0; i < numberOfSamples; ++i, ++iterator)
    {
        auto generatedNumber = *iterator;
        if (generatedNumber < lowerBoundry || generatedNumber > upperBoundry)
        {
            generatedNumbersAreWithinBoundries = false;
            break;
        }
        ++buckets[(generatedNumber - 1) / bucketSize];
    }

    REQUIRE(generatedNumbersAreWithinBoundries);

    auto numbersAreUniformlyDistributed = true;
    for (auto i = 0; i < numberOfBuckets; ++i)
        if (std::abs(idealSamplesPerBucket - buckets[i]) / static_cast<double>(idealSamplesPerBucket) > tolerance)
        {
            numbersAreUniformlyDistributed = false;
            break;
        }

    REQUIRE(numbersAreUniformlyDistributed);
}
