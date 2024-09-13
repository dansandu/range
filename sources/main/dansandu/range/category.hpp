#pragma once

#include "dansandu/ballotin/type_traits.hpp"

namespace dansandu::range::category
{

struct range_binder_tag
{
    range_binder_tag() = default;

    range_binder_tag(const range_binder_tag&) = delete;

    range_binder_tag(range_binder_tag&&) = default;

    range_binder_tag& operator=(const range_binder_tag&) = delete;

    range_binder_tag& operator=(range_binder_tag&&) = default;
};

template<typename Type>
struct is_pipe_head_wrapper
{
    template<typename = Type>
    static constexpr auto getValue(dansandu::ballotin::type_traits::general_case)
    {
        return false;
    }

    template<typename T = Type, typename B = decltype(std::declval<T>().begin()),
             typename E = decltype(std::declval<T>().end())>
    static constexpr auto getValue(dansandu::ballotin::type_traits::best_case)
    {
        return true;
    }
};

template<typename Type>
inline constexpr auto is_pipe_head =
    is_pipe_head_wrapper<typename std::decay_t<Type>>::getValue(dansandu::ballotin::type_traits::best_case{});

template<typename Type>
inline constexpr auto is_range_binder = std::is_base_of_v<range_binder_tag, std::decay_t<Type>>;

}
