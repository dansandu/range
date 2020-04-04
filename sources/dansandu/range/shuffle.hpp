#pragma once

#include "dansandu/range/category.hpp"

#include <algorithm>
#include <type_traits>
#include <vector>

namespace dansandu::range::shuffle
{

template<typename Generator>
class ShuffleBinder : public dansandu::range::category::range_binder_tag
{
public:
    ShuffleBinder(Generator* generator) : generator_{generator}
    {
    }

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
        std::shuffle(result.begin(), result.end(), *generator_);
        return result;
    }

private:
    Generator* generator_;
};

template<typename Generator>
inline auto shuffle(Generator& generator)
{
    using generator_type = std::decay_t<Generator>;
    return ShuffleBinder<generator_type>{&generator};
}

}
