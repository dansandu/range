#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iterator>
#include <type_traits>
#include <utility>

namespace dansandu::range::zip {

template<typename LeftIterator, typename RightIterator>
class ZipIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::pair<std::decay_t<decltype(*std::declval<LeftIterator>())>,
                                 std::decay_t<decltype(*std::declval<RightIterator>())>>;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = long long;

    friend bool operator==(const ZipIterator& a, const ZipIterator& b) {
        return a.leftPosition_ == b.leftPosition_ || a.rightPosition_ == b.rightPosition_;
    }

    ZipIterator(LeftIterator leftPosition, LeftIterator leftEnd, RightIterator rightPosition, RightIterator rightEnd)
        : leftPosition_{std::move(leftPosition)},
          leftEnd_{std::move(leftEnd)},
          rightPosition_{std::move(rightPosition)},
          rightEnd_{std::move(rightEnd)} {}

    ZipIterator(const ZipIterator&) = default;

    ZipIterator(ZipIterator&&) = default;

    ZipIterator& operator=(const ZipIterator&) = default;

    ZipIterator& operator=(ZipIterator&&) = default;

    ZipIterator& operator++() {
        if (leftPosition_ != leftEnd_ && rightPosition_ != rightEnd_) {
            ++leftPosition_;
            ++rightPosition_;
        }
        return *this;
    }

    ZipIterator operator++(int) {
        auto copy = *this;
        ++*this;
        return copy;
    }

    auto operator*() const { return value_type{*leftPosition_, *rightPosition_}; }

private:
    LeftIterator leftPosition_;
    LeftIterator leftEnd_;
    RightIterator rightPosition_;
    RightIterator rightEnd_;
};

template<typename LeftIterator, typename RightIterator>
bool operator!=(const ZipIterator<LeftIterator, RightIterator>& a, const ZipIterator<LeftIterator, RightIterator>& b) {
    return !(a == b);
}

template<typename LeftRange, typename RightRange>
struct ZipRange {
public:
    using range_category = dansandu::range::category::view_tag;
    using left_range_storage = dansandu::range::storage::Storage<LeftRange>;
    using right_range_storage = dansandu::range::storage::Storage<RightRange>;
    using const_iterator =
        ZipIterator<typename left_range_storage::const_iterator, typename right_range_storage::const_iterator>;
    using iterator = const_iterator;

    template<typename LeftRangeForward, typename RightRangeForward>
    ZipRange(LeftRangeForward&& leftRange, RightRangeForward&& rightRange)
        : leftRange_{std::forward<LeftRangeForward>(leftRange)},
          rightRange_{std::forward<RightRangeForward>(rightRange)} {}

    ZipRange(const ZipRange&) = delete;

    ZipRange(ZipRange&&) = default;

    ZipRange& operator=(const ZipRange&) = delete;

    ZipRange& operator=(ZipRange&&) = default;

    auto cbegin() const {
        return const_iterator{leftRange_.cbegin(), leftRange_.cend(), rightRange_.cbegin(), rightRange_.cend()};
    }

    auto cend() const {
        return const_iterator{leftRange_.cend(), leftRange_.cend(), rightRange_.cend(), rightRange_.cend()};
    }

    auto begin() const { return cbegin(); }

    auto end() const { return cend(); }

private:
    left_range_storage leftRange_;
    right_range_storage rightRange_;
};

template<typename RightRange>
class ZipBinder : public dansandu::range::category::range_binder_tag {
public:
    template<typename RightRangeForward>
    explicit ZipBinder(RightRangeForward&& rightRange) : rightRange_{std::forward<RightRangeForward>(rightRange)} {}

    ZipBinder(const ZipBinder&) = delete;

    ZipBinder(ZipBinder&&) = default;

    ZipBinder& operator=(const ZipBinder&) = delete;

    ZipBinder& operator=(ZipBinder&&) = default;

    template<typename LeftRange>
    auto bind(LeftRange&& leftRange) && {
        return ZipRange<LeftRange&&, RightRange&&>{std::forward<LeftRange>(leftRange), std::move(rightRange_)};
    }

private:
    dansandu::range::storage::Storage<RightRange> rightRange_;
};

template<typename RightRange, typename = std::enable_if_t<dansandu::range::category::is_pipe_head<RightRange>>>
auto zip(RightRange&& rightRange) {
    return ZipBinder<RightRange&&>{std::forward<RightRange>(rightRange)};
}

}
