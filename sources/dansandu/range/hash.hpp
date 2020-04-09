#pragma once

#include "dansandu/ballotin/hash.hpp"
#include "dansandu/range/category.hpp"

namespace dansandu::range::hash
{

class HashBinder : public dansandu::range::category::range_binder_tag
{
public:
    HashBinder() = default;

    HashBinder(const HashBinder&) = delete;

    HashBinder(HashBinder&&) = default;

    HashBinder& operator=(const HashBinder&) = delete;

    HashBinder& operator=(HashBinder&&) = default;

    template<typename InputRange>
    auto bind(InputRange&& inputRange) &&
    {
        auto seed = std::size_t{};
        for (const auto& element : inputRange)
            seed = dansandu::ballotin::hash::hashCombine(seed, element);
        return seed;
    }
};

auto hash()
{
    return HashBinder{};
}

}
