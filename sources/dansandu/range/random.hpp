#pragma once

#include "dansandu/range/category.hpp"

#include <iterator>
#include <random>

namespace dansandu::range::random
{

template<typename ValueType, bool isIntegral = std::is_integral_v<ValueType>>
class DistributionWrapper;

template<typename ValueType>
class DistributionWrapper<ValueType, true>
{
public:
    DistributionWrapper(const ValueType& a, const ValueType& b) : distribution_{a, b}
    {
    }

    template<typename Generator>
    auto operator()(Generator& generator)
    {
        return distribution_(generator);
    }

private:
    std::uniform_int_distribution<ValueType> distribution_;
};

template<typename ValueType>
class DistributionWrapper<ValueType, false>
{
public:
    DistributionWrapper(const ValueType& a, const ValueType& b) : distribution_{a, b}
    {
    }

    template<typename Generator>
    auto operator()(Generator& generator)
    {
        return distribution_(generator);
    }

private:
    std::uniform_real_distribution<ValueType> distribution_;
};

template<typename Generator, typename ValueType>
class RandomIterator
{
public:
    using generator_type = Generator;
    using value_type = ValueType;
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using reference = value_type&;
    using pointer = value_type*;

    RandomIterator(generator_type* generator, const value_type& a, const value_type& b)
        : generator_{generator}, distributionWrapper_{a, b}, value_{distributionWrapper_(*generator)}
    {
    }

    RandomIterator(const RandomIterator&) = default;

    RandomIterator(RandomIterator&&) = default;

    RandomIterator& operator=(const RandomIterator&) = default;

    RandomIterator& operator=(RandomIterator&&) = default;

    auto& operator++()
    {
        value_ = distributionWrapper_(*generator_);
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
    generator_type* generator_;
    DistributionWrapper<value_type> distributionWrapper_;
    value_type value_;
};

template<typename Generator, typename ValueType>
auto operator==(const RandomIterator<Generator, ValueType>&, const RandomIterator<Generator, ValueType>&)
{
    return false;
}

template<typename Generator, typename ValueType>
auto operator!=(const RandomIterator<Generator, ValueType>&, const RandomIterator<Generator, ValueType>&)
{
    return true;
}

template<typename Generator, typename ValueType>
class RandomRange
{
public:
    using generator_type = Generator;
    using value_type = ValueType;
    using range_category = dansandu::range::category::generator_tag;
    using const_iterator = RandomIterator<generator_type, value_type>;
    using iterator = const_iterator;

    RandomRange(generator_type* generator, const ValueType& a, const ValueType& b) : generator_{generator}, a_{a}, b_{b}
    {
    }

    RandomRange(const RandomRange&) = delete;

    RandomRange(RandomRange&&) = default;

    RandomRange& operator=(const RandomRange&) = delete;

    RandomRange& operator=(RandomRange&&) = default;

    auto cbegin() const
    {
        return const_iterator{generator_, a_, b_};
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
        return cbegin();
    }

private:
    generator_type* generator_;
    value_type a_;
    value_type b_;
};

template<typename ValueType, typename Generator>
auto random(Generator& generator, const ValueType& a, const ValueType& b)
{
    using generator_type = std::decay_t<Generator>;
    using value_type = std::decay_t<ValueType>;
    static_assert(std::is_integral_v<value_type> || std::is_floating_point_v<value_type>);
    return RandomRange<generator_type, value_type>{&generator, a, b};
}

template<typename ValueType, typename Generator>
auto random(Generator& generator)
{
    using generator_type = std::decay_t<Generator>;
    using value_type = std::decay_t<ValueType>;
    static_assert(std::is_integral_v<value_type> || std::is_floating_point_v<value_type>);
    if constexpr (std::is_integral_v<value_type>)
        return RandomRange<generator_type, value_type>(&generator, 0, std::numeric_limits<value_type>::max());
    else
        return RandomRange<generator_type, value_type>(&generator, 0.0, 1.0);
}

}
