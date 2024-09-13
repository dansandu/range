#pragma once

#include "dansandu/ballotin/hash.hpp"
#include "dansandu/range/category.hpp"

namespace dansandu::range::hash
{

class HashBinder : public dansandu::range::category::range_binder_tag
{
public:
    template<typename InputRange>
    auto bind(InputRange&& inputRange) &&
    {
        auto seed = std::size_t{};
        for (auto&& element : inputRange)
            seed = dansandu::ballotin::hash::hashCombine(seed, std::forward<decltype(element)>(element));
        return seed;
    }
};

auto hash()
{
    return HashBinder{};
}

}
