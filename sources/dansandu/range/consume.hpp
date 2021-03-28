#pragma once

#include "dansandu/range/category.hpp"

#include <type_traits>

namespace dansandu::range::consume
{

template<typename Consumer>
class ConsumeBinder : public dansandu::range::category::range_binder_tag
{
public:
    template<typename C>
    ConsumeBinder(C&& consumer) : consumer_{std::forward<C>(consumer)}
    {
    }

    template<typename InputRange>
    auto bind(InputRange&& inputRange) &&
    {
        for (auto&& element : inputRange)
            consumer_(std::forward<decltype(element)>(element));
    }

private:
    Consumer consumer_;
};

template<typename Consumer>
auto consume(Consumer&& consumer)
{
    using consumer_type = std::decay_t<Consumer>;
    return ConsumeBinder<consumer_type>{std::forward<Consumer>(consumer)};
}

}
