#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iterator>
#include <type_traits>

namespace dansandu::range::map {

template<typename InputIterator, typename MappingPointer>
class MapIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using value_type =
        typename std::decay_t<decltype((*std::declval<MappingPointer>())(*std::declval<InputIterator>()))>;
    using reference = value_type&;
    using pointer = value_type*;

    friend bool operator==(const MapIterator& a, const MapIterator& b) { return a.position_ == b.position_; }

    MapIterator(InputIterator position, MappingPointer mapping) : position_{std::move(position)}, mapping_{mapping} {}

    explicit MapIterator(InputIterator position) : position_{std::move(position)}, mapping_{nullptr} {}

    MapIterator(const MapIterator&) = default;

    MapIterator(MapIterator&&) = default;

    MapIterator& operator=(const MapIterator&) = default;

    MapIterator& operator=(MapIterator&&) = default;

    auto& operator++() {
        ++position_;
        return *this;
    }

    auto operator++(int) {
        auto copy = *this;
        ++*this;
        return copy;
    }

    auto operator*() const { return (*mapping_)(*position_); }

private:
    InputIterator position_;
    MappingPointer mapping_;
};

template<typename InputIterator, typename Mapping>
bool operator!=(const MapIterator<InputIterator, Mapping>& a, const MapIterator<InputIterator, Mapping>& b) {
    return !(a == b);
}

template<typename InputRange, typename Mapping>
class MapRange {
public:
    using range_category = dansandu::range::category::view_tag;
    using range_storage = dansandu::range::storage::Storage<InputRange>;
    using mapping_pointer = std::conditional_t<std::is_pointer_v<Mapping>, Mapping, Mapping*>;
    using const_iterator = MapIterator<typename range_storage::const_iterator, mapping_pointer>;
    using iterator = const_iterator;

    template<typename InputRangeForward, typename MappingForward>
    MapRange(InputRangeForward&& inputRange, MappingForward&& mapping)
        : storage_{std::forward<InputRangeForward>(inputRange)}, mapping_{std::forward<MappingForward>(mapping)} {}

    MapRange(const MapRange&) = delete;

    MapRange(MapRange&&) = default;

    MapRange& operator=(const MapRange&) = delete;

    MapRange& operator=(MapRange&&) = default;

    auto cbegin() const {
        if constexpr (std::is_pointer_v<Mapping>)
            return const_iterator{storage_.cbegin(), mapping_};
        else
            return const_iterator{storage_.cbegin(), &mapping_};
    }

    auto cend() const { return const_iterator{storage_.cend()}; }

    auto begin() const { return cbegin(); }

    auto end() const { return cend(); }

private:
    range_storage storage_;
    mutable Mapping mapping_;
};

template<typename Mapping>
class MapBinder : public dansandu::range::category::range_binder_tag {
public:
    using decayed_mapping = std::decay_t<Mapping>;

    template<typename MappingForward>
    MapBinder(MappingForward&& mapping) : mapping_{std::forward<MappingForward>(mapping)} {}

    MapBinder(const MapBinder&) = delete;

    MapBinder(MapBinder&&) = default;

    MapBinder& operator=(const MapBinder&) = delete;

    MapBinder& operator=(MapBinder&&) = default;

    template<typename InputRange>
    auto bind(InputRange&& inputRange) && {
        return MapRange<InputRange&&, decayed_mapping>{std::forward<InputRange>(inputRange), std::move(mapping_)};
    }

private:
    decayed_mapping mapping_;
};

template<typename Mapping>
inline auto map(Mapping&& mapping) {
    return MapBinder<Mapping&&>{std::forward<Mapping>(mapping)};
}

}
