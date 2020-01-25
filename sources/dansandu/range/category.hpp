#pragma once

#include "dansandu/ballotin/type_traits.hpp"

#include <type_traits>
#include <vector>

namespace dansandu::range::category
{

struct container_tag
{
};

struct generator_tag
{
};

struct view_tag
{
};

struct reduction_tag
{
};

template<typename Type>
struct is_pipe_head_wrapper
{
    template<typename = Type>
    static constexpr auto getValue(dansandu::ballotin::type_traits::general_case)
    {
        return false;
    }

    template<typename T = Type, auto value = dansandu::ballotin::type_traits::type_pack<
                                    container_tag, generator_tag, view_tag>::contains<typename T::range_category>>
    static constexpr auto getValue(dansandu::ballotin::type_traits::best_case)
    {
        return value;
    }
};

template<typename Type>
struct is_pipe_head_wrapper<std::vector<Type>>
{
    static constexpr auto getValue(dansandu::ballotin::type_traits::best_case)
    {
        return true;
    }
};

template<typename Type>
inline constexpr auto is_pipe_head =
    is_pipe_head_wrapper<typename std::decay_t<Type>>::getValue(dansandu::ballotin::type_traits::best_case{});

struct range_binder_tag
{
};

template<typename Type>
inline constexpr auto is_range_binder = std::is_base_of_v<range_binder_tag, std::decay_t<Type>>;

}
