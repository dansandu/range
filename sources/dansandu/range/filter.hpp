#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/storage.hpp"

#include <iterator>
#include <type_traits>

namespace dansandu::range::filter
{

template<typename InputIterator, typename PredicatePointer>
class FilterIterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::decay_t<decltype(*std::declval<InputIterator>())>;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = long long;

    friend auto operator==(const FilterIterator& a, const FilterIterator& b)
    {
        return a.position_ == b.position_;
    }

    FilterIterator(InputIterator position, InputIterator end)
        : position_{std::move(position)}, end_{std::move(end)}, predicate_{nullptr}
    {
    }

    FilterIterator(InputIterator position, InputIterator end, PredicatePointer predicate)
        : position_{std::move(position)}, end_{std::move(end)}, predicate_{predicate}
    {
        findMatch();
    }

    FilterIterator(const FilterIterator&) = default;

    FilterIterator(FilterIterator&&) = default;

    FilterIterator& operator=(const FilterIterator&) = default;

    FilterIterator& operator=(FilterIterator&&) = default;

    auto& operator++()
    {
        ++position_;
        findMatch();
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
        return *position_;
    }

private:
    void findMatch()
    {
        if (predicate_)
        {
            if constexpr (std::is_member_function_pointer_v<PredicatePointer>)
                while (position_ != end_ && !(((*position_).*predicate_)()))
                    ++position_;
            else if constexpr (std::is_member_object_pointer_v<PredicatePointer>)
                while (position_ != end_ && !((*position_).*predicate_))
                    ++position_;
            else
                while (position_ != end_ && !((*predicate_)(*position_)))
                    ++position_;
        }
        else
            position_ = end_;
    }

    InputIterator position_;
    InputIterator end_;
    PredicatePointer predicate_;
};

template<typename Iterator, typename Predicate>
auto operator!=(const FilterIterator<Iterator, Predicate>& a, const FilterIterator<Iterator, Predicate>& b)
{
    return !(a == b);
}

template<typename InputRange, typename Predicate>
class FilterRange
{
public:
    using range_storage = dansandu::range::storage::Storage<InputRange>;
    using decayed_predicate = std::decay_t<Predicate>;
    using predicate_pointer =
        std::conditional_t<std::is_pointer_v<decayed_predicate> || std::is_member_pointer_v<decayed_predicate>,
                           decayed_predicate, decayed_predicate*>;
    using const_iterator = FilterIterator<typename range_storage::const_iterator, predicate_pointer>;
    using iterator = const_iterator;

    template<typename InputRangeForward, typename PredicateForward>
    FilterRange(InputRangeForward&& inputRange, PredicateForward&& predicate)
        : inputRange_{std::forward<InputRangeForward>(inputRange)},
          predicate_{std::forward<PredicateForward>(predicate)}
    {
    }

    FilterRange(const FilterRange&) = delete;

    FilterRange(FilterRange&&) = default;

    FilterRange& operator=(const FilterRange&) = delete;

    FilterRange& operator=(FilterRange&&) = default;

    auto cbegin() const
    {
        if constexpr (std::is_pointer_v<decayed_predicate> || std::is_member_pointer_v<decayed_predicate>)
            return const_iterator{inputRange_.begin(), inputRange_.end(), predicate_};
        else
            return const_iterator{inputRange_.begin(), inputRange_.end(), &predicate_};
    }

    auto cend() const
    {
        return const_iterator{inputRange_.end(), inputRange_.end()};
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
    range_storage inputRange_;
    mutable decayed_predicate predicate_;
};

template<typename Predicate>
class FilterBinder : public dansandu::range::category::range_binder_tag
{
public:
    using decayed_predicate = typename std::decay_t<Predicate>;

    template<typename PredicateForward>
    explicit FilterBinder(PredicateForward&& predicate) : predicate_{std::forward<PredicateForward>(predicate)}
    {
    }

    template<typename InputRange>
    auto bind(InputRange&& inputRange) &&
    {
        return FilterRange<InputRange&&, decayed_predicate>{std::forward<InputRange>(inputRange),
                                                            std::move(predicate_)};
    }

private:
    decayed_predicate predicate_;
};

template<typename Predicate>
auto filter(Predicate&& predicate)
{
    return FilterBinder<Predicate&&>(std::forward<Predicate>(predicate));
}

}
