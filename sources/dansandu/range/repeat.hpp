#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iterator>
#include <type_traits>

namespace dansandu::range::repeat {

template<typename Iterator>
class RepeatIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using value_type = typename std::decay_t<decltype(*std::declval<Iterator>())>;
    using pointer = value_type*;
    using reference = value_type&;

    friend auto operator==(const RepeatIterator& a, const RepeatIterator& b) { return a.position_ == b.position_; }

    RepeatIterator(Iterator begin, Iterator position, Iterator end)
        : begin_{std::move(begin)}, position_{std::move(position)}, end_{std::move(end)} {}

    RepeatIterator(const RepeatIterator&) = default;

    RepeatIterator(RepeatIterator&&) = default;

    RepeatIterator& operator=(const RepeatIterator&) = default;

    RepeatIterator& operator=(RepeatIterator&&) = default;

    auto& operator++() {
        ++position_;
        if (position_ == end_)
            position_ = begin_;
        return *this;
    }

    auto operator++(int) {
        auto copy = *this;
        ++*this;
        return copy;
    }

    auto operator*() const { return *position_; }

private:
    Iterator begin_;
    Iterator position_;
    Iterator end_;
};

template<typename Iterator>
auto operator!=(const RepeatIterator<Iterator>& a, const RepeatIterator<Iterator>& b) {
    return !(a == b);
}

template<typename InputRange>
class RepeatView {
public:
    using range_storage_type = dansandu::range::storage::Storage<InputRange>;
    using range_category = dansandu::range::category::view_tag;
    using const_iterator = RepeatIterator<typename range_storage_type::const_iterator>;
    using iterator = const_iterator;

    template<typename InputRangeForward>
    explicit RepeatView(InputRangeForward&& inputRange) : inputRange_{std::forward<InputRangeForward>(inputRange)} {}

    RepeatView(const RepeatView&) = default;

    RepeatView(RepeatView&&) = default;

    RepeatView& operator=(const RepeatView&) = default;

    RepeatView& operator=(RepeatView&&) = default;

    auto cbegin() const { return iterator{inputRange_.cbegin(), inputRange_.cbegin(), inputRange_.cend()}; }

    auto cend() const { return iterator{inputRange_.cbegin(), inputRange_.cend(), inputRange_.cend()}; }

    auto begin() const { return cbegin(); }

    auto end() const { return cend(); }

private:
    range_storage_type inputRange_;
};

class RepeatViewFactory {
public:
    using range_factory_category = dansandu::range::category::view_factory_tag;

    template<typename InputRange>
    auto create(InputRange&& inputRange) && {
        return RepeatView<InputRange&&>{std::forward<InputRange>(inputRange)};
    }
};

inline auto repeat() { return RepeatViewFactory{}; }

}
