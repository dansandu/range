#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iterator>
#include <type_traits>

namespace dansandu::range::concatenate
{

template<typename LeftIterator, typename RightIterator>
class ConcatenateIterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using value_type = std::decay_t<decltype(*std::declval<LeftIterator>())>;
    using reference = value_type&;
    using pointer = value_type*;

    friend auto operator==(const ConcatenateIterator& a, const ConcatenateIterator& b)
    {
        return a.leftPosition_ == b.leftPosition_ && a.rightPosition_ == b.rightPosition_;
    }

    ConcatenateIterator(LeftIterator leftPosition, LeftIterator leftEnd, RightIterator rightPosition)
        : leftPosition_{std::move(leftPosition)}, leftEnd_{std::move(leftEnd)}, rightPosition_{std::move(rightPosition)}
    {
    }

    ConcatenateIterator(const ConcatenateIterator&) = default;

    ConcatenateIterator(ConcatenateIterator&&) = default;

    ConcatenateIterator& operator=(const ConcatenateIterator&) = default;

    ConcatenateIterator& operator=(ConcatenateIterator&&) = default;

    auto& operator++()
    {
        if (leftPosition_ != leftEnd_)
            ++leftPosition_;
        else
            ++rightPosition_;
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
        if (leftPosition_ != leftEnd_)
            return *leftPosition_;
        else
            return value_type{*rightPosition_};
    }

private:
    LeftIterator leftPosition_;
    LeftIterator leftEnd_;
    RightIterator rightPosition_;
};

template<typename LeftIterator, typename RightIterator>
auto operator!=(const ConcatenateIterator<LeftIterator, RightIterator>& a,
                const ConcatenateIterator<LeftIterator, RightIterator>& b)
{
    return !(a == b);
}

template<typename LeftRange, typename RightRange>
class ConcatenateRange
{
public:
    using range_category = dansandu::range::category::view_tag;
    using left_range_storage = dansandu::range::storage::Storage<LeftRange>;
    using right_range_storage = dansandu::range::storage::Storage<RightRange>;
    using const_iterator =
        ConcatenateIterator<typename left_range_storage::const_iterator, typename right_range_storage::const_iterator>;
    using iterator = const_iterator;

    template<typename LeftRangeForward, typename RightRangeForward>
    ConcatenateRange(LeftRangeForward&& leftRange, RightRangeForward&& rightRange)
        : leftRange_{std::forward<LeftRangeForward>(leftRange)},
          rightRange_{std::forward<RightRangeForward>(rightRange)}
    {
    }

    ConcatenateRange(const ConcatenateRange&) = delete;

    ConcatenateRange(ConcatenateRange&&) = default;

    ConcatenateRange& operator=(const ConcatenateRange&) = delete;

    ConcatenateRange& operator=(ConcatenateRange&&) = default;

    auto cbegin() const
    {
        return const_iterator{leftRange_.cbegin(), leftRange_.cend(), rightRange_.cbegin()};
    }

    auto cend() const
    {
        return const_iterator{leftRange_.cend(), leftRange_.cend(), rightRange_.cend()};
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
    left_range_storage leftRange_;
    right_range_storage rightRange_;
};

template<typename RightRange>
class ConcatenateBinder : public dansandu::range::category::range_binder_tag
{
public:
    template<typename RightRangeForward>
    explicit ConcatenateBinder(RightRangeForward&& rightRange)
        : rightRange_{std::forward<RightRangeForward>(rightRange)}
    {
    }

    ConcatenateBinder(const ConcatenateBinder&) = delete;

    ConcatenateBinder(ConcatenateBinder&&) = default;

    ConcatenateBinder& operator=(const ConcatenateBinder&) = delete;

    ConcatenateBinder& operator=(ConcatenateBinder&&) = default;

    template<typename LeftRange>
    auto bind(LeftRange&& leftRange) &&
    {
        return ConcatenateRange<LeftRange&&, RightRange&&>{std::forward<LeftRange>(leftRange), std::move(rightRange_)};
    }

private:
    dansandu::range::storage::Storage<RightRange> rightRange_;
};

template<typename InputRange>
inline auto concatenate(InputRange&& inputRange)
{
    return ConcatenateBinder<InputRange&&>{std::forward<InputRange>(inputRange)};
}

}
