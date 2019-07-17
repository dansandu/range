#pragma once

#include "summer_school/range/category.hpp"
#include "summer_school/range/storage.hpp"

#include <iterator>
#include <type_traits>
#include <utility>

namespace summer_school::range::zip {

template<typename LeftIterator, typename RightIterator>
class ZipIterator {
public:
    friend bool operator==(ZipIterator a, ZipIterator b) {
        return ((a.leftPosition_ == b.leftPosition_) & (a.rightPosition_ == b.rightPosition_)) |
               ((a.leftPosition_ == a.leftEnd_) & (b.leftPosition_ == b.leftEnd_)) |
               ((a.leftPosition_ == a.leftEnd_) & (b.rightPosition_ == b.rightEnd_)) |
               ((a.rightPosition_ == a.rightEnd_) & (b.leftPosition_ == b.leftEnd_)) |
               ((a.rightPosition_ == a.rightEnd_) & (b.rightPosition_ == b.rightEnd_));
    }

    using difference_type = long long;
    using value_type = decltype(std::make_pair(*std::declval<LeftIterator>(), *std::declval<RightIterator>()));
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::input_iterator_tag;

    ZipIterator(LeftIterator leftPosition, LeftIterator leftEnd, RightIterator rightPosition, RightIterator rightEnd)
        : leftPosition_{leftPosition}, leftEnd_{leftEnd}, rightPosition_{rightPosition}, rightEnd_{rightEnd} {}

    ZipIterator(const ZipIterator&) = default;

    ZipIterator(ZipIterator&&) noexcept = default;

    ZipIterator& operator=(const ZipIterator&) = default;

    ZipIterator& operator=(ZipIterator&&) noexcept = default;

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

    value_type operator*() { return {*leftPosition_, *rightPosition_}; }

private:
    LeftIterator leftPosition_;
    LeftIterator leftEnd_;
    RightIterator rightPosition_;
    RightIterator rightEnd_;
};

template<typename LeftIterator, typename RightIterator>
bool operator!=(ZipIterator<LeftIterator, RightIterator> a, ZipIterator<LeftIterator, RightIterator> b) {
    return !(a == b);
}

template<typename LeftRange, typename RightRange>
struct ZipView {
public:
    using const_iterator = ZipIterator<typename summer_school::range::storage::Storage<LeftRange>::const_iterator,
                                       typename summer_school::range::storage::Storage<RightRange>::const_iterator>;
    using iterator = const_iterator;
    using range_category = summer_school::range::category::view_tag;

    template<typename Left, typename Right>
    ZipView(Left&& left, Right&& right)
        : leftRange_{std::forward<Left>(left)}, rightRange_{std::forward<Right>(right)} {}

    ZipView(const ZipView&) = default;

    ZipView(ZipView&&) noexcept = default;

    ZipView& operator=(const ZipView&) = default;

    ZipView& operator=(ZipView&&) noexcept = default;

    auto cbegin() const {
        return iterator{leftRange_.cbegin(), leftRange_.cend(), rightRange_.cbegin(), rightRange_.cend()};
    }

    auto cend() const { return iterator{leftRange_.cend(), leftRange_.cend(), rightRange_.cend(), rightRange_.cend()}; }

    auto begin() const { return cbegin(); }

    auto end() const { return cend(); }

private:
    summer_school::range::storage::Storage<LeftRange> leftRange_;
    summer_school::range::storage::Storage<RightRange> rightRange_;
};

template<typename LeftView, typename RightView>
auto zip(LeftView&& leftView, RightView&& rightView) {
    return ZipView<LeftView&&, RightView&&>{std::forward<LeftView>(leftView), std::forward<RightView>(rightView)};
}

}
