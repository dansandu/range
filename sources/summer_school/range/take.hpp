#pragma once

#include "summer_school/range/category.hpp"
#include "summer_school/range/storage.hpp"

#include <iterator>
#include <type_traits>
#include <utility>

namespace summer_school::range::take {

template<typename Iterator>
class TakeIterator {
public:
    friend auto operator==(TakeIterator a, TakeIterator b) {
        return a.position_ == b.position_ ||
               (a.elementsTaken_ == a.elementsToTake_ && b.elementsTaken_ == b.elementsToTake_);
    }

    using difference_type = long long;
    using value_type = std::decay_t<decltype(*std::declval<Iterator>())>;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::input_iterator_tag;

    TakeIterator(Iterator position, Iterator end, int elementsToTake)
        : position_{position}, end_{end}, elementsTaken_{0}, elementsToTake_{elementsToTake} {}

    TakeIterator(const TakeIterator&) = default;

    TakeIterator(TakeIterator&&) noexcept = default;

    TakeIterator& operator=(const TakeIterator&) = default;

    TakeIterator& operator=(TakeIterator&&) noexcept = default;

    auto& operator++() {
        if (elementsTaken_ < elementsToTake_) {
            ++position_;
            ++elementsTaken_;
        }
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
    int elementsTaken_;
    int elementsToTake_;
};

template<typename Iterator>
auto operator!=(TakeIterator<Iterator> a, TakeIterator<Iterator> b) {
    return !(a == b);
}

template<typename InputRange>
class TakeView {
public:
    using range_storage_type = summer_school::range::storage::Storage<InputRange>;
    using const_iterator = TakeIterator<typename range_storage_type::const_iterator>;
    using iterator = const_iterator;
    using range_category = summer_school::range::category::view_tag;

    template<typename InputRangeForward>
    TakeView(InputRangeForward&& inputRange, int elementsToTake)
        : inputRange_{std::forward<InputRangeForward>(inputRange)}, elementsToTake_{elementsToTake} {}

    TakeView(const TakeView&) = delete;

    TakeView(TakeView&&) noexcept = default;

    TakeView& operator=(const TakeView&) = delete;

    TakeView& operator=(TakeView&&) noexcept = default;

    auto cbegin() const { return iterator{inputRange_.cbegin(), inputRange_.cend(), elementsToTake_}; }

    auto cend() const { return iterator{inputRange_.cend(), inputRange_.cend(), 0}; }

    auto begin() const { return cbegin(); }

    auto end() const { return cend(); }

private:
    range_storage_type inputRange_;
    int elementsToTake_;
};

class TakeViewFactory {
public:
    using range_factory_category = summer_school::range::category::view_factory_tag;

    explicit TakeViewFactory(int elementsToTake) : elementsToTake_{elementsToTake} {}

    template<typename InputRange>
    auto create(InputRange&& inputRange) && {
        return TakeView<InputRange&&>{std::forward<InputRange>(inputRange), elementsToTake_};
    }

private:
    int elementsToTake_;
};

inline auto take(int elements) { return TakeViewFactory{elements}; }

}