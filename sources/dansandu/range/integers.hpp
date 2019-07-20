#pragma once

#include "dansandu/range/category.hpp"

#include <iterator>
#include <limits>

namespace dansandu::range::integers {

class IntegersIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using value_type = int;
    using pointer = value_type*;
    using reference = value_type&;

    IntegersIterator(value_type integer, value_type step) : integer_{integer}, step_{step} {}

    IntegersIterator(const IntegersIterator&) = default;

    IntegersIterator(IntegersIterator&&) = default;

    IntegersIterator& operator=(const IntegersIterator&) = default;

    IntegersIterator& operator=(IntegersIterator&&) = default;

    auto& operator++() {
        integer_ += step_;
        return *this;
    }

    auto operator++(int) {
        auto copy = *this;
        ++*this;
        return copy;
    }

    auto operator*() const { return integer_; }

private:
    value_type integer_;
    value_type step_;
};

inline auto operator==(IntegersIterator a, IntegersIterator b) { return *a == *b; }

inline auto operator!=(IntegersIterator a, IntegersIterator b) { return !(a == b); }

class IntegersGenerator {
public:
    using range_category = dansandu::range::category::generator_tag;
    using const_iterator = IntegersIterator;
    using iterator = IntegersIterator;
    using value_type = IntegersIterator::value_type;

    IntegersGenerator(value_type start, value_type step, value_type count)
        : start_{start}, step_{step}, count_{count} {}

    IntegersGenerator(const IntegersGenerator&) = delete;

    IntegersGenerator(IntegersGenerator&&) = default;

    IntegersGenerator& operator=(const IntegersGenerator&) = delete;

    IntegersGenerator& operator=(IntegersGenerator&&) = default;

    auto cbegin() const { return const_iterator{start_, step_}; }

    auto cend() const { return const_iterator{start_ + step_ * count_, step_}; }

    auto begin() const { return cbegin(); }

    auto end() const { return cend(); }

private:
    value_type start_;
    value_type step_;
    value_type count_;
};

inline auto integers(IntegersGenerator::value_type start, IntegersGenerator::value_type step,
                     IntegersGenerator::value_type count) {
    return IntegersGenerator{start, step, count};
}

inline auto integers(IntegersGenerator::value_type start, IntegersGenerator::value_type step) {
    using safe_type = long long;
    static_assert(sizeof(IntegersGenerator::value_type) < sizeof(safe_type));
    auto count = static_cast<IntegersGenerator::value_type>(
        (static_cast<safe_type>(std::numeric_limits<IntegersGenerator::value_type>::max()) -
         static_cast<safe_type>(start)) /
        static_cast<safe_type>(step));
    return IntegersGenerator{start, step, count};
}

inline auto integers(IntegersGenerator::value_type start) { return integers(start, 1); }

inline auto integers() { return integers(0, 1); }

}
