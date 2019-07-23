#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iterator>
#include <type_traits>
#include <utility>

namespace dansandu::range::take {

template<typename InputIterator>
class TakeIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::decay_t<decltype(*std::declval<InputIterator>())>;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = long long;

    friend auto operator==(const TakeIterator& a, const TakeIterator& b) {
        return a.position_ == b.position_ ||
               (a.elementsTaken_ == a.elementsToTake_ && b.elementsTaken_ == b.elementsToTake_);
    }

    TakeIterator(InputIterator position, InputIterator end, int elementsToTake)
        : position_{std::move(position)}, end_{std::move(end)}, elementsTaken_{0}, elementsToTake_{elementsToTake} {}

    TakeIterator(const TakeIterator&) = default;

    TakeIterator(TakeIterator&&) = default;

    TakeIterator& operator=(const TakeIterator&) = default;

    TakeIterator& operator=(TakeIterator&&) = default;

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
    InputIterator position_;
    InputIterator end_;
    int elementsTaken_;
    int elementsToTake_;
};

template<typename InputIterator>
auto operator!=(const TakeIterator<InputIterator>& a, const TakeIterator<InputIterator>& b) {
    return !(a == b);
}

template<typename InputRange>
class TakeRange {
public:
    using range_category = dansandu::range::category::view_tag;
    using range_storage = dansandu::range::storage::Storage<InputRange>;
    using const_iterator = TakeIterator<typename range_storage::const_iterator>;
    using iterator = const_iterator;

    template<typename InputRangeForward>
    TakeRange(InputRangeForward&& inputRange, int elementsToTake)
        : inputRange_{std::forward<InputRangeForward>(inputRange)}, elementsToTake_{elementsToTake} {}

    TakeRange(const TakeRange&) = delete;

    TakeRange(TakeRange&&) = default;

    TakeRange& operator=(const TakeRange&) = delete;

    TakeRange& operator=(TakeRange&&) = default;

    auto cbegin() const { return iterator{inputRange_.cbegin(), inputRange_.cend(), elementsToTake_}; }

    auto cend() const { return iterator{inputRange_.cend(), inputRange_.cend(), 0}; }

    auto begin() const { return cbegin(); }

    auto end() const { return cend(); }

private:
    range_storage inputRange_;
    int elementsToTake_;
};

class TakeBinder : public dansandu::range::category::range_binder_tag {
public:
    explicit TakeBinder(int elementsToTake) : elementsToTake_{elementsToTake} {}

    TakeBinder(const TakeBinder&) = delete;

    TakeBinder(TakeBinder&&) = default;

    TakeBinder& operator=(const TakeBinder&) = delete;

    TakeBinder& operator=(TakeBinder&&) = default;

    template<typename InputRange>
    auto bind(InputRange&& inputRange) && {
        return TakeRange<InputRange&&>{std::forward<InputRange>(inputRange), elementsToTake_};
    }

private:
    int elementsToTake_;
};

inline auto take(int elements) { return TakeBinder{elements}; }

}
