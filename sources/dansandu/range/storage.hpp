#pragma once

#include <type_traits>

namespace dansandu::range::storage {

enum class Store { pointer, move };

template<typename Range, Store = std::is_lvalue_reference_v<Range> ? Store::pointer : Store::move>
class Storage;

template<typename Range>
class Storage<Range, Store::pointer> {
public:
    using decayed_type = typename std::decay_t<Range>;
    using const_iterator = typename decayed_type::const_iterator;
    using iterator = const_iterator;

    Storage(const decayed_type& range) : range_{&range} {}

    iterator cbegin() const { return range_->cbegin(); }

    iterator cend() const { return range_->cend(); }

    iterator begin() const { return range_->begin(); }

    iterator end() const { return range_->end(); }

private:
    const decayed_type* range_;
};

template<typename Range>
class Storage<Range, Store::move> {
public:
    using decayed_type = typename std::decay_t<Range>;
    using const_iterator = typename decayed_type::const_iterator;
    using iterator = const_iterator;

    Storage(decayed_type&& range) : range_{std::move(range)} {}

    Storage(const Storage&) = default;

    Storage(Storage&&) noexcept = default;

    Storage& operator=(const Storage&) = default;

    Storage& operator=(Storage&&) noexcept = default;

    iterator cbegin() const { return range_.cbegin(); }

    iterator cend() const { return range_.cend(); }

    iterator begin() const { return range_.begin(); }

    iterator end() const { return range_.end(); }

private:
    decayed_type range_;
};

}
