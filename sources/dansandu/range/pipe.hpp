#pragma once

#include "dansandu/range/category.hpp"

#include <type_traits>

namespace dansandu::range::pipe {

template<typename InputRange, typename RangeFactory,
         typename = std::enable_if_t<dansandu::range::category::is_pipe_head<InputRange> &&
                                     dansandu::range::category::is_range_factory<RangeFactory>>>
auto operator|(InputRange&& inputRange, RangeFactory&& rangeFactory) {
    return std::forward<RangeFactory>(rangeFactory).create(std::forward<InputRange>(inputRange));
}

}
