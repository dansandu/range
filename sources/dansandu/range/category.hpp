#pragma once

#include <type_traits>
#include <vector>

namespace dansandu::range::category {

struct container_tag {};

struct generator_tag {};

struct view_tag {};

struct reduction_tag {};

struct view_factory_tag {};

struct reduction_factory_tag {};

template<typename... Types>
struct type_pack {
    template<typename Type>
    static inline constexpr bool contains = (... || std::is_same_v<Type, Types>);
};

struct general_case {};

struct best_case : general_case {};

template<typename Type>
struct is_pipe_head_wrapper {
    template<typename = Type>
    static constexpr bool getValue(general_case) {
        return false;
    }

    template<typename T = Type,
             bool value = type_pack<container_tag, generator_tag, view_tag>::contains<typename T::range_category>>
    static constexpr bool getValue(best_case) {
        return value;
    }
};

template<typename Type>
struct is_pipe_head_wrapper<std::vector<Type>> {
    static constexpr bool getValue(best_case) { return true; }
};

template<typename Type>
struct is_range_factory_wrapper {
    template<typename = Type>
    static constexpr bool getValue(general_case) {
        return false;
    }

    template<typename T = Type, bool value = type_pack<view_factory_tag, reduction_factory_tag>::contains<
                                    typename T::range_factory_category>>
    static constexpr bool getValue(best_case) {
        return value;
    }
};

template<typename Type>
inline constexpr bool is_pipe_head = is_pipe_head_wrapper<typename std::decay_t<Type>>::getValue(best_case{});

template<typename Type>
inline constexpr bool is_range_factory = is_range_factory_wrapper<typename std::decay_t<Type>>::getValue(best_case{});

template<typename InputRange, typename RangeFactory,
         typename = std::enable_if_t<is_pipe_head<InputRange> && is_range_factory<RangeFactory>>>
auto operator|(InputRange&& inputRange, RangeFactory&& rangeFactory) {
    return std::forward<RangeFactory>(rangeFactory).create(std::forward<InputRange>(inputRange));
}

}
