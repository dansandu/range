#pragma once

#include "dansandu/range/category.hpp"
#include "dansandu/range/randomness.hpp"

#include <algorithm>
#include <type_traits>
#include <vector>

namespace dansandu::range::shuffle
{

class ShuffleBinder : public dansandu::range::category::range_binder_tag
{
public:
    ShuffleBinder() = default;

    ShuffleBinder(const ShuffleBinder&) = delete;

    ShuffleBinder(ShuffleBinder&&) = default;

    ShuffleBinder& operator=(const ShuffleBinder&) = delete;

    ShuffleBinder& operator=(ShuffleBinder&&) = default;

    template<typename InputRange>
    auto bind(InputRange&& inputRange) &&
    {
        using value_type = typename std::decay_t<decltype(*std::forward<InputRange>(inputRange).cbegin())>;
        auto result = std::vector<value_type>{};
        for (auto&& element : inputRange)
            result.push_back(std::forward<decltype(element)>(element));
        std::shuffle(result.begin(), result.end(), dansandu::range::randomness::getRandomGenerator());
        return result;
    }
};

inline auto shuffle()
{
    return ShuffleBinder{};
}

}
