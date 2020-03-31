#pragma once

#include "dansandu/ballotin/default_random_generator.hpp"
#include "dansandu/range/category.hpp"

#include <iostream>
#include <type_traits>
#include <vector>

namespace dansandu::range::shuffle
{

template<typename RandomGenerator>
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
        auto size = static_cast<int>(result.size());
        using result_type = typename std::decay_t<decltype(RandomGenerator::instance())>::result_type;
        for (auto index = 0; index + 1 < size; ++index)
        {
            auto range = static_cast<result_type>(size - index);
            auto pick = index + static_cast<int>(RandomGenerator::instance()() % range);
            using std::swap;
            swap(result[index], result[pick]);
        }
        return result;
    }
};

template<typename RandomGenerator = dansandu::ballotin::default_random_generator::DefaultRandomGenerator>
inline auto shuffle()
{
    return ShuffleBinder<RandomGenerator>{};
}

}
