#pragma once

#include "dansandu/range/category.hpp"

#include <type_traits>
#include <vector>

namespace dansandu::range::to_vector
{

class ToVectorBinder : public dansandu::range::category::range_binder_tag
{
public:
    template<typename InputRange>
    auto bind(InputRange&& inputRange) &&
    {
        using value_type = typename std::decay_t<decltype(*std::forward<InputRange>(inputRange).begin())>;
        auto result = std::vector<value_type>{};
        for (auto&& element : inputRange)
            result.push_back(std::forward<decltype(element)>(element));
        return result;
    }
};

inline auto toVector()
{
    return ToVectorBinder{};
}

}
