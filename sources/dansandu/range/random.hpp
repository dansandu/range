#pragma once

#include "dansandu/ballotin/default_random_generator.hpp"
#include "dansandu/range/category.hpp"

#include <iterator>

namespace dansandu::range::random
{

template<typename T, bool isIntegral = std::is_integral_v<T>>
class DistributionWrapper;

template<typename T>
class DistributionWrapper<T, true>
{
public:
    DistributionWrapper(const T& a, const T& b) : distribution{a, b}
    {
    }

    template<typename Generator>
    auto operator()(Generator& generator)
    {
        return distribution(generator);
    }

private:
    std::uniform_int_distribution<T> distribution;
};

template<typename T>
class DistributionWrapper<T, false>
{
public:
    DistributionWrapper(const T& a, const T& b) : distribution{a, b}
    {
    }

    template<typename Generator>
    auto operator()(Generator& generator)
    {
        return distribution(generator);
    }

private:
    std::uniform_real_distribution<T> distribution;
};

template<typename T, typename RandomGenerator>
class RandomIterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using value_type = T;
    using reference = value_type&;
    using pointer = value_type*;

    RandomIterator(const value_type& lowerBoundry, const value_type& upperBoundry)
        : distributionWrapper_{lowerBoundry, upperBoundry}, value_{distributionWrapper_(RandomGenerator::instance())}
    {
    }

    RandomIterator(const RandomIterator&) = default;

    RandomIterator(RandomIterator&&) = default;

    RandomIterator& operator=(const RandomIterator&) = default;

    RandomIterator& operator=(RandomIterator&&) = default;

    auto& operator++()
    {
        value_ = distributionWrapper_(RandomGenerator::instance());
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
    DistributionWrapper<value_type> distributionWrapper_;
    value_type value_;
};

template<typename T, typename R1, typename R2>
inline auto operator==(const RandomIterator<T, R1>&, const RandomIterator<T, R2>&)
{
    return false;
}

template<typename T, typename R1, typename R2>
inline auto operator!=(const RandomIterator<T, R1>&, const RandomIterator<T, R2>&)
{
    return true;
}

template<typename T, typename RandomGenerator>
class RandomRange
{
public:
    using value_type = std::decay_t<T>;
    using range_category = dansandu::range::category::generator_tag;
    using const_iterator = RandomIterator<value_type, RandomGenerator>;
    using iterator = const_iterator;

    RandomRange(value_type lowerBoundry, value_type upperBoundry)
        : lowerBoundry_{lowerBoundry}, upperBoundry_{upperBoundry}
    {
    }

    RandomRange(const RandomRange&) = delete;

    RandomRange(RandomRange&&) = default;

    RandomRange& operator=(const RandomRange&) = delete;

    RandomRange& operator=(RandomRange&&) = default;

    auto cbegin() const
    {
        return const_iterator{lowerBoundry_, upperBoundry_};
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
    value_type lowerBoundry_;
    value_type upperBoundry_;
};

template<typename T, typename RandomGenerator = dansandu::ballotin::default_random_generator::DefaultRandomGenerator>
auto random(const T& lowerBoundry, const T& upperBoundry)
{
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);
    return RandomRange<T, RandomGenerator>{lowerBoundry, upperBoundry};
}

template<typename T, typename RandomGenerator = dansandu::ballotin::default_random_generator::DefaultRandomGenerator>
auto random()
{
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);
    if constexpr (std::is_integral_v<T>)
    {
        return RandomRange<T, RandomGenerator>(0, 100);
    }
    else
    {
        return RandomRange<T, RandomGenerator>(0.0, 1.0);
    }
}

}
