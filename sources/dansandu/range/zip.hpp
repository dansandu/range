#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iterator>
#include <type_traits>
#include <utility>

namespace dansandu::range::zip
{

template<typename ZipperPointer, typename LeftIterator, typename RightIterator>
class ZipIterator
{
    static decltype(auto) zip(ZipperPointer zipper, const LeftIterator& left, const RightIterator& right)
    {
        if constexpr (std::is_member_function_pointer_v<ZipperPointer>)
            return ((*left).*zipper)(*right);
        else
            return (*zipper)(*left, *right);
    }

public:
    using iterator_category = std::input_iterator_tag;
    using value_type = typename std::decay_t<decltype(
        zip(std::declval<ZipperPointer>(), std::declval<LeftIterator>(), std::declval<RightIterator>()))>;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = long long;

    friend auto operator==(const ZipIterator& a, const ZipIterator& b)
    {
        return a.leftPosition_ == b.leftPosition_ || a.rightPosition_ == b.rightPosition_;
    }

    ZipIterator(ZipperPointer zipper, LeftIterator leftPosition, LeftIterator leftEnd, RightIterator rightPosition,
                RightIterator rightEnd)
        : zipper_{zipper},
          leftPosition_{std::move(leftPosition)},
          leftEnd_{std::move(leftEnd)},
          rightPosition_{std::move(rightPosition)},
          rightEnd_{std::move(rightEnd)}
    {
    }

    ZipIterator(const ZipIterator&) = default;

    ZipIterator(ZipIterator&&) = default;

    ZipIterator& operator=(const ZipIterator&) = default;

    ZipIterator& operator=(ZipIterator&&) = default;

    ZipIterator& operator++()
    {
        if (leftPosition_ != leftEnd_ && rightPosition_ != rightEnd_)
        {
            ++leftPosition_;
            ++rightPosition_;
        }
        return *this;
    }

    ZipIterator operator++(int)
    {
        auto copy = *this;
        ++*this;
        return copy;
    }

    decltype(auto) operator*() const
    {
        return zip(zipper_, leftPosition_, rightPosition_);
    }

private:
    ZipperPointer zipper_;
    LeftIterator leftPosition_;
    LeftIterator leftEnd_;
    RightIterator rightPosition_;
    RightIterator rightEnd_;
};

template<typename ZipperPointer, typename LeftIterator, typename RightIterator>
auto operator!=(const ZipIterator<ZipperPointer, LeftIterator, RightIterator>& a,
                const ZipIterator<ZipperPointer, LeftIterator, RightIterator>& b)
{
    return !(a == b);
}

template<typename Zipper, typename LeftRange, typename RightRange>
struct ZipRange
{
public:
    using decayed_zipper = std::decay_t<Zipper>;
    using zipper_pointer =
        std::conditional_t<std::is_pointer_v<decayed_zipper> || std::is_member_pointer_v<decayed_zipper>,
                           decayed_zipper, decayed_zipper*>;
    using left_range_storage = dansandu::range::storage::Storage<LeftRange>;
    using right_range_storage = dansandu::range::storage::Storage<RightRange>;
    using const_iterator = ZipIterator<zipper_pointer, typename left_range_storage::const_iterator,
                                       typename right_range_storage::const_iterator>;
    using iterator = const_iterator;

    template<typename ZipperForward, typename LeftRangeForward, typename RightRangeForward>
    ZipRange(ZipperForward&& zipper, LeftRangeForward&& leftRange, RightRangeForward&& rightRange)
        : zipper_{std::forward<ZipperForward>(zipper)},
          leftRange_{std::forward<LeftRangeForward>(leftRange)},
          rightRange_{std::forward<RightRangeForward>(rightRange)}
    {
    }

    ZipRange(const ZipRange&) = delete;

    ZipRange(ZipRange&&) = default;

    ZipRange& operator=(const ZipRange&) = delete;

    ZipRange& operator=(ZipRange&&) = default;

    auto cbegin() const
    {
        if constexpr (std::is_pointer_v<decayed_zipper> || std::is_member_pointer_v<decayed_zipper>)
            return const_iterator{zipper_, leftRange_.begin(), leftRange_.end(), rightRange_.begin(),
                                  rightRange_.end()};
        else
            return const_iterator{&zipper_, leftRange_.begin(), leftRange_.end(), rightRange_.begin(),
                                  rightRange_.end()};
    }

    auto cend() const
    {
        if constexpr (std::is_pointer_v<decayed_zipper> || std::is_member_pointer_v<decayed_zipper>)
            return const_iterator{zipper_, leftRange_.end(), leftRange_.end(), rightRange_.end(), rightRange_.end()};
        else
            return const_iterator{&zipper_, leftRange_.end(), leftRange_.end(), rightRange_.end(), rightRange_.end()};
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
    mutable decayed_zipper zipper_;
    left_range_storage leftRange_;
    right_range_storage rightRange_;
};

template<typename Zipper, typename RightRange>
class ZipBinder : public dansandu::range::category::range_binder_tag
{
public:
    using decayed_zipper = std::decay_t<Zipper>;

    template<typename ZipperForward, typename RightRangeForward>
    explicit ZipBinder(ZipperForward&& zipper, RightRangeForward&& rightRange)
        : zipper_{std::forward<ZipperForward>(zipper)}, rightRange_{std::forward<RightRangeForward>(rightRange)}
    {
    }

    template<typename LeftRange>
    auto bind(LeftRange&& leftRange) &&
    {
        return ZipRange<decayed_zipper, LeftRange&&, RightRange&&>{
            std::move(zipper_), std::forward<LeftRange>(leftRange), std::move(rightRange_)};
    }

private:
    decayed_zipper zipper_;
    dansandu::range::storage::Storage<RightRange> rightRange_;
};

template<typename Zipper, typename RightRange,
         typename = std::enable_if_t<dansandu::range::category::is_pipe_head<RightRange>>>
auto zip(Zipper&& zipper, RightRange&& rightRange)
{
    return ZipBinder<Zipper&&, RightRange&&>{std::forward<Zipper>(zipper), std::forward<RightRange>(rightRange)};
}

struct pair
{
    template<typename T1, typename T2>
    constexpr auto operator()(T1&& first, T2&& second) const
    {
        return std::make_pair(std::forward<T1>(first), std::forward<T2>(second));
    }
};

template<typename RightRange, typename = std::enable_if_t<dansandu::range::category::is_pipe_head<RightRange>>>
auto zip(RightRange&& rightRange)
{
    return zip(pair{}, std::forward<RightRange>(rightRange));
}

}
