#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iostream>
#include <iterator>
#include <type_traits>

namespace dansandu::range::filter {

template<typename Iterator, typename Predicate>
class FilterIterator {
public:
    using range_category = dansandu::range::category::view_tag;
    using iterator_category = std::input_iterator_tag;
    using value_type = std::decay_t<decltype(*std::declval<Iterator>())>;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = long long;

    friend auto operator==(const FilterIterator& a, const FilterIterator& b) { return a.position_ == b.position_; }

    FilterIterator(Iterator position, Iterator end, Predicate* predicate)
        : position_{std::move(position)}, end_{std::move(end)}, predicate_{predicate} {}

    FilterIterator(Iterator position, Iterator end)
        : position_{std::move(position)}, end_{std::move(end)}, predicate_{nullptr} {}

    FilterIterator(const FilterIterator&) = default;

    FilterIterator(FilterIterator&&) = default;

    FilterIterator& operator=(const FilterIterator&) = default;

    FilterIterator& operator=(FilterIterator&&) = default;

    auto& operator++() {
        ++position_;
        if (predicate_)
            while (position_ != end_ && !((*predicate_)(*position_)))
                ++position_;
        else
            position_ = end_;
        return *this;
    }

    auto operator++(int) {
        auto copy = *this;
        ++*this;
        return copy;
    }

    auto operator*() const { return *position_; }

private:
    Iterator position_;
    Iterator end_;
    Predicate* predicate_;
};

template<typename Iterator, typename Predicate>
auto operator!=(const FilterIterator<Iterator, Predicate>& a, const FilterIterator<Iterator, Predicate>& b) {
    return !(a == b);
}

template<typename InputRange, typename Predicate>
class FilterView {
public:
    using range_category = dansandu::range::category::view_tag;
    using range_storage_type = dansandu::range::storage::Storage<InputRange>;
    using const_iterator = FilterIterator<typename range_storage_type::const_iterator, Predicate>;
    using iterator = const_iterator;

    template<typename InputRangeForward, typename PredicateForward>
    FilterView(InputRangeForward&& inputRange, PredicateForward&& predicate)
        : inputRange_{std::forward<InputRangeForward>(inputRange)},
          predicate_{std::forward<PredicateForward>(predicate)} {}

    FilterView(const FilterView&) = delete;

    FilterView(FilterView&&) = default;

    FilterView& operator=(const FilterView&) = delete;

    FilterView& operator=(FilterView&&) = default;

    auto cbegin() const { return iterator{inputRange_.cbegin(), inputRange_.cend(), &predicate_}; }

    auto cend() const { return iterator{inputRange_.cend(), inputRange_.cend()}; }

    auto begin() const { return cbegin(); }

    auto end() const { return cend(); }

private:
    range_storage_type inputRange_;
    mutable Predicate predicate_;
};

template<typename Predicate>
class FilterViewFactory {
public:
    using range_factory_category = dansandu::range::category::view_factory_tag;
    using predicate_type = typename std::remove_pointer_t<typename std::decay_t<Predicate>>;

    template<typename PredicateForward>
    explicit FilterViewFactory(PredicateForward&& predicate) : predicate_{std::forward<PredicateForward>(predicate)} {}

    FilterViewFactory(const FilterViewFactory&) = delete;

    FilterViewFactory(FilterViewFactory&&) = default;

    FilterViewFactory& operator=(const FilterViewFactory&) = delete;

    FilterViewFactory& operator=(FilterViewFactory&&) = default;

    template<typename InputRange>
    auto create(InputRange&& inputRange) && {
        return FilterView<InputRange&&, predicate_type>{std::forward<InputRange>(inputRange), std::move(predicate_)};
    }

private:
    predicate_type predicate_;
};

template<typename Predicate>
auto filter(Predicate&& predicate) {
    return FilterViewFactory<Predicate&&>(std::forward<Predicate>(predicate));
}

}
