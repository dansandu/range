#pragma once

#include "dansandu/range/category.hpp"

#include <type_traits>

namespace dansandu::range::for_each
{

template<typename Consumer>
class ForEachBinder : public dansandu::range::category::range_binder_tag
{
public:
    template<typename C>
    ForEachBinder(C&& consumer) : consumer_{std::forward<C>(consumer)}
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
auto forEach(Consumer&& consumer)
{
    using consumer_type = std::decay_t<Consumer>;
    return ForEachBinder<consumer_type>{std::forward<Consumer>(consumer)};
}

}
