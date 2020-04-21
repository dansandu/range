#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iterator>
#include <type_traits>

namespace dansandu::range::cycle
{

template<typename InputIterator>
class CycleIterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using value_type = typename std::decay_t<decltype(*std::declval<InputIterator>())>;
    using pointer = value_type*;
    using reference = value_type&;

    friend auto operator==(const CycleIterator& a, const CycleIterator& b)
    {
        return a.position_ == b.position_;
    }

    CycleIterator(InputIterator begin, InputIterator position, InputIterator end)
        : begin_{std::move(begin)}, position_{std::move(position)}, end_{std::move(end)}
    {
    }

    CycleIterator(const CycleIterator&) = default;

    CycleIterator(CycleIterator&&) = default;

    CycleIterator& operator=(const CycleIterator&) = default;

    CycleIterator& operator=(CycleIterator&&) = default;

    auto& operator++()
    {
        if (position_ == end_)
        {
            position_ = begin_;
        }
        else
        {
            ++position_;
            if (position_ == end_)
                position_ = begin_;
        }
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
        return *position_;
    }

private:
    InputIterator begin_;
    InputIterator position_;
    InputIterator end_;
};

template<typename InputIterator>
auto operator!=(const CycleIterator<InputIterator>& a, const CycleIterator<InputIterator>& b)
{
    return !(a == b);
}

template<typename InputRange>
class CycleRange
{
public:
    using range_storage = dansandu::range::storage::Storage<InputRange>;
    using const_iterator = CycleIterator<typename range_storage::const_iterator>;
    using iterator = const_iterator;

    template<typename InputRangeForward>
    explicit CycleRange(InputRangeForward&& inputRange) : inputRange_{std::forward<InputRangeForward>(inputRange)}
    {
    }

    CycleRange(const CycleRange&) = default;

    CycleRange(CycleRange&&) = default;

    CycleRange& operator=(const CycleRange&) = default;

    CycleRange& operator=(CycleRange&&) = default;

    auto cbegin() const
    {
        return const_iterator{inputRange_.begin(), inputRange_.begin(), inputRange_.end()};
    }

    auto cend() const
    {
        return const_iterator{inputRange_.begin(), inputRange_.end(), inputRange_.end()};
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
    range_storage inputRange_;
};

class CycleBinder : public dansandu::range::category::range_binder_tag
{
public:
    CycleBinder(const CycleBinder&) = delete;

    CycleBinder(CycleBinder&&) = default;

    CycleBinder& operator=(const CycleBinder&) = delete;

    CycleBinder& operator=(CycleBinder&&) = default;

    template<typename InputRange>
    auto bind(InputRange&& inputRange) &&
    {
        return CycleRange<InputRange&&>{std::forward<InputRange>(inputRange)};
    }
};

inline auto cycle()
{
    return CycleBinder{};
}

}
