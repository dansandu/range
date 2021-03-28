#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iterator>
#include <type_traits>

namespace dansandu::range::map
{

template<typename InputIterator, typename MappingPointer>
class MapIterator
{
    static decltype(auto) map(const InputIterator& position, MappingPointer mapping)
    {
        if constexpr (std::is_member_function_pointer_v<MappingPointer>)
            return ((*position).*mapping)();
        else if constexpr (std::is_member_object_pointer_v<MappingPointer>)
            return (*position).*mapping;
        else
            return (*mapping)(*position);
    }

public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = long long;
    using value_type =
        typename std::decay_t<decltype(map(std::declval<InputIterator>(), std::declval<MappingPointer>()))>;
    using reference = value_type&;
    using pointer = value_type*;

    friend auto operator==(const MapIterator& a, const MapIterator& b)
    {
        return a.position_ == b.position_;
    }

    explicit MapIterator(InputIterator position) : position_{std::move(position)}, mapping_{nullptr}
    {
    }

    MapIterator(InputIterator position, MappingPointer mapping) : position_{std::move(position)}, mapping_{mapping}
    {
    }

    MapIterator(const MapIterator&) = default;

    MapIterator(MapIterator&&) = default;

    MapIterator& operator=(const MapIterator&) = default;

    MapIterator& operator=(MapIterator&&) = default;

    auto& operator++()
    {
        ++position_;
        return *this;
    }

    auto operator++(int)
    {
        auto copy = *this;
        ++*this;
        return copy;
    }

    decltype(auto) operator*() const
    {
        return map(position_, mapping_);
    }

private:
    InputIterator position_;
    MappingPointer mapping_;
};

template<typename InputIterator, typename Mapping>
auto operator!=(const MapIterator<InputIterator, Mapping>& a, const MapIterator<InputIterator, Mapping>& b)
{
    return !(a == b);
}

template<typename InputRange, typename Mapping>
class MapRange
{
public:
    using range_storage = dansandu::range::storage::Storage<InputRange>;
    using mapping_pointer =
        std::conditional_t<std::is_pointer_v<Mapping> || std::is_member_pointer_v<Mapping>, Mapping, Mapping*>;
    using const_iterator = MapIterator<typename range_storage::const_iterator, mapping_pointer>;
    using iterator = const_iterator;

    template<typename InputRangeForward, typename MappingForward>
    MapRange(InputRangeForward&& inputRange, MappingForward&& mapping)
        : storage_{std::forward<InputRangeForward>(inputRange)}, mapping_{std::forward<MappingForward>(mapping)}
    {
    }

    MapRange(const MapRange&) = delete;

    MapRange(MapRange&&) = default;

    MapRange& operator=(const MapRange&) = delete;

    MapRange& operator=(MapRange&&) = default;

    auto cbegin() const
    {
        if constexpr (std::is_pointer_v<Mapping> || std::is_member_pointer_v<Mapping>)
            return const_iterator{storage_.begin(), mapping_};
        else
            return const_iterator{storage_.begin(), &mapping_};
    }

    auto cend() const
    {
        return const_iterator{storage_.end()};
    }

    auto begin() const
    {
        return cbegin();
    }

    auto end() const
    {
        return cend();
    }

private:
    range_storage storage_;
    mutable Mapping mapping_;
};

template<typename Mapping>
class MapBinder : public dansandu::range::category::range_binder_tag
{
public:
    using decayed_mapping = std::decay_t<Mapping>;

    template<typename MappingForward>
    MapBinder(MappingForward&& mapping) : mapping_{std::forward<MappingForward>(mapping)}
    {
    }

    template<typename InputRange>
    auto bind(InputRange&& inputRange) &&
    {
        return MapRange<InputRange&&, decayed_mapping>{std::forward<InputRange>(inputRange), std::move(mapping_)};
    }

private:
    decayed_mapping mapping_;
};

template<typename Mapping>
inline auto map(Mapping&& mapping)
{
    return MapBinder<Mapping&&>{std::forward<Mapping>(mapping)};
}

}
