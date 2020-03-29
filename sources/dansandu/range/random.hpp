#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/randomness.hpp"

#include <iterator>
#include <random>

namespace dansandu::range::random
{

class RandomIterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using value_type = int;
    using reference = value_type&;
    using pointer = value_type*;

    RandomIterator(value_type lowerBoundryInclusive, value_type upperBoundryInclusive)
        : distribution_{lowerBoundryInclusive, upperBoundryInclusive},
          value_{distribution_(dansandu::range::randomness::getRandomGenerator())}
    {
    }

    RandomIterator(const RandomIterator&) = default;

    RandomIterator(RandomIterator&&) = default;

    RandomIterator& operator=(const RandomIterator&) = default;

    RandomIterator& operator=(RandomIterator&&) = default;

    auto& operator++()
    {
        value_ = distribution_(dansandu::range::randomness::getRandomGenerator());
        return *this;
    }

    auto operator++(int)
    {
        auto copy = *this;
        ++*this;
        return copy;
    }

    auto operator*() const
    {
        return value_;
    }

private:
    std::uniform_int_distribution<value_type> distribution_;
    value_type value_;
};

inline auto operator==(const RandomIterator&, const RandomIterator&)
{
    return false;
}

inline auto operator!=(const RandomIterator&, const RandomIterator&)
{
    return true;
}

class RandomRange
{
public:
    using range_category = dansandu::range::category::generator_tag;
    using const_iterator = RandomIterator;
    using iterator = const_iterator;
    using value_type = const_iterator::value_type;

    RandomRange(value_type lowerBoundryInclusive, value_type upperBoundryInclusive)
        : lowerBoundryInclusive_{lowerBoundryInclusive}, upperBoundryInclusive_{upperBoundryInclusive}
    {
    }

    RandomRange(const RandomRange&) = delete;

    RandomRange(RandomRange&&) = default;

    RandomRange& operator=(const RandomRange&) = delete;

    RandomRange& operator=(RandomRange&&) = default;

    auto cbegin() const
    {
        return const_iterator{lowerBoundryInclusive_, upperBoundryInclusive_};
    }

    auto cend() const
    {
        return cbegin();
    }

    auto begin() const
    {
        return cbegin();
    }

    auto end() const
    {
        return cend();
    }

private:
    value_type lowerBoundryInclusive_;
    value_type upperBoundryInclusive_;
};

inline auto random(RandomRange::value_type lowerBoundryInclusive, RandomRange::value_type upperBoundryInclusive)
{
    return RandomRange{lowerBoundryInclusive, upperBoundryInclusive};
}

inline auto random()
{
    return RandomRange{1, 100};
}

}
