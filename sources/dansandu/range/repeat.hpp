#pragma once

#include "dansandu/range/category.hpp"

#include <iterator>

namespace dansandu::range::repeat
{

template<typename ValueType>
class RepeatIterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using value_type = ValueType;
    using pointer = value_type*;
    using reference = value_type&;

    RepeatIterator(const value_type* value) : value_{value}
    {
    }

    RepeatIterator(const RepeatIterator&) = default;

    RepeatIterator(RepeatIterator&&) = default;

    RepeatIterator& operator=(const RepeatIterator&) = default;

    RepeatIterator& operator=(RepeatIterator&&) = default;

    auto& operator++()
    {
        return *this;
    }

    auto operator++(int)
    {
        auto copy = *this;
        ++*this;
        return copy;
    }

    const auto& operator*() const
    {
        return *value_;
    }

private:
    const value_type* value_;
};

template<typename ValueType>
auto operator==(const RepeatIterator<ValueType>&, const RepeatIterator<ValueType>&)
{
    return false;
}

template<typename ValueType>
auto operator!=(const RepeatIterator<ValueType>&, const RepeatIterator<ValueType>&)
{
    return false;
}

template<typename ValueType>
class RepeatRange
{
public:
    using value_type = ValueType;
    using const_iterator = RepeatIterator<value_type>;
    using iterator = const_iterator;

    template<typename V>
    RepeatRange(V&& value) : value_{std::forward<V>(value)}
    {
    }

    RepeatRange(const RepeatRange&) = delete;

    RepeatRange(RepeatRange&&) noexcept = default;

    RepeatRange& operator=(const RepeatRange&) = delete;

    RepeatRange& operator=(RepeatRange&&) noexcept = default;

    auto cbegin() const
    {
        return const_iterator{&value_};
    }

    auto cend() const
    {
        return const_iterator{&value_};
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
    value_type value_;
};

template<typename ValueType>
inline auto repeat(ValueType&& value)
{
    using value_type = std::decay_t<ValueType>;
    return RepeatRange<value_type>{std::forward<ValueType>(value)};
}

}
