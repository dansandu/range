#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iterator>
#include <type_traits>

namespace dansandu::range::repeat
{

template<typename InputIterator>
class RepeatIterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using value_type = typename std::decay_t<decltype(*std::declval<InputIterator>())>;
    using pointer = value_type*;
    using reference = value_type&;

    friend auto operator==(const RepeatIterator& a, const RepeatIterator& b)
    {
        return a.position_ == b.position_;
    }

    RepeatIterator(InputIterator begin, InputIterator position, InputIterator end)
        : begin_{std::move(begin)}, position_{std::move(position)}, end_{std::move(end)}
    {
    }

    RepeatIterator(const RepeatIterator&) = default;

    RepeatIterator(RepeatIterator&&) = default;

    RepeatIterator& operator=(const RepeatIterator&) = default;

    RepeatIterator& operator=(RepeatIterator&&) = default;

    auto& operator++()
    {
        ++position_;
        if (position_ == end_)
            position_ = begin_;
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
auto operator!=(const RepeatIterator<InputIterator>& a, const RepeatIterator<InputIterator>& b)
{
    return !(a == b);
}

template<typename InputRange>
class RepeatRange
{
public:
    using range_storage = dansandu::range::storage::Storage<InputRange>;
    using range_category = dansandu::range::category::view_tag;
    using const_iterator = RepeatIterator<typename range_storage::const_iterator>;
    using iterator = const_iterator;

    template<typename InputRangeForward>
    explicit RepeatRange(InputRangeForward&& inputRange) : inputRange_{std::forward<InputRangeForward>(inputRange)}
    {
    }

    RepeatRange(const RepeatRange&) = default;

    RepeatRange(RepeatRange&&) = default;

    RepeatRange& operator=(const RepeatRange&) = default;

    RepeatRange& operator=(RepeatRange&&) = default;

    auto cbegin() const
    {
        return const_iterator{inputRange_.cbegin(), inputRange_.cbegin(), inputRange_.cend()};
    }

    auto cend() const
    {
        return const_iterator{inputRange_.cbegin(), inputRange_.cend(), inputRange_.cend()};
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

class RepeatBinder : public dansandu::range::category::range_binder_tag
{
public:
    RepeatBinder(const RepeatBinder&) = delete;

    RepeatBinder(RepeatBinder&&) = default;

    RepeatBinder& operator=(const RepeatBinder&) = delete;

    RepeatBinder& operator=(RepeatBinder&&) = default;

    template<typename InputRange>
    auto bind(InputRange&& inputRange) &&
    {
        return RepeatRange<InputRange&&>{std::forward<InputRange>(inputRange)};
    }
};

inline auto repeat()
{
    return RepeatBinder{};
}

}
