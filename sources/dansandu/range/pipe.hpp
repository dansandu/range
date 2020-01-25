#pragma once

#include "dansandu/range/category.hpp"

#include <type_traits>

namespace dansandu::range::pipe
{

template<typename InputRange, typename RangeBinder,
         typename = std::enable_if_t<dansandu::range::category::is_pipe_head<InputRange> &&
                                     dansandu::range::category::is_range_binder<RangeBinder>>>
auto operator|(InputRange&& inputRange, RangeBinder&& rangeBinder)
{
    return std::forward<RangeBinder>(rangeBinder).bind(std::forward<InputRange>(inputRange));
}

}
