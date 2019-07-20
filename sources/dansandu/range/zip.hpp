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
    friend bool operator==(const ZipIterator& a, const ZipIterator& b) {
        return a.leftPosition_ == b.leftPosition_ || a.rightPosition_ == b.rightPosition_;
    }

    using iterator_category = std::input_iterator_tag;
    using value_type = std::pair<std::decay_t<decltype(*std::declval<LeftIterator>())>,
                                 std::decay_t<decltype(*std::declval<RightIterator>())>>;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = long long;

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
struct ZipView {
public:
    using range_category = dansandu::range::category::view_tag;
    using left_range_storage_type = dansandu::range::storage::Storage<LeftRange>;
    using right_range_storage_type = dansandu::range::storage::Storage<RightRange>;
    using const_iterator = ZipIterator<typename left_range_storage_type::const_iterator,
                                       typename right_range_storage_type::const_iterator>;
    using iterator = const_iterator;

    template<typename LeftRangeForward, typename RightRangeForward>
    ZipView(LeftRangeForward&& leftRange, RightRangeForward&& rightRange)
        : leftRange_{std::forward<LeftRangeForward>(leftRange)},
          rightRange_{std::forward<RightRangeForward>(rightRange)} {}

    ZipView(const ZipView&) = delete;

    ZipView(ZipView&&) = default;

    ZipView& operator=(const ZipView&) = delete;

    ZipView& operator=(ZipView&&) = default;

    auto cbegin() const {
        return iterator{leftRange_.cbegin(), leftRange_.cend(), rightRange_.cbegin(), rightRange_.cend()};
    }

    auto cend() const { return iterator{leftRange_.cend(), leftRange_.cend(), rightRange_.cend(), rightRange_.cend()}; }

    auto begin() const { return cbegin(); }

    auto end() const { return cend(); }

private:
    left_range_storage_type leftRange_;
    right_range_storage_type rightRange_;
};

template<typename RightRange>
class ZipViewFactory {
public:
    using range_factory_category = dansandu::range::category::view_factory_tag;

    template<typename RightRangeForward>
    explicit ZipViewFactory(RightRangeForward&& rightRange)
        : rightRange_{std::forward<RightRangeForward>(rightRange)} {}

    ZipViewFactory(const ZipViewFactory&) = delete;

    ZipViewFactory(ZipViewFactory&&) = default;

    ZipViewFactory& operator=(const ZipViewFactory&) = delete;

    ZipViewFactory& operator=(ZipViewFactory&&) = default;

    template<typename LeftRange>
    auto create(LeftRange&& leftRange) && {
        return ZipView<LeftRange&&, RightRange&&>{std::forward<LeftRange>(leftRange), std::move(rightRange_)};
    }

private:
    dansandu::range::storage::Storage<RightRange> rightRange_;
};

template<typename RightRange, typename = std::enable_if_t<dansandu::range::category::is_pipe_head<RightRange>>>
auto zip(RightRange&& rightRange) {
    return ZipViewFactory<RightRange&&>{std::forward<RightRange>(rightRange)};
}

}
