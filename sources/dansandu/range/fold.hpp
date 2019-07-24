#pragma once

#include "dansandu/range/category.hpp"

#include <type_traits>

namespace dansandu::range::fold {

template<typename InitialValue, typename Folder>
class FoldBinder : public dansandu::range::category::range_binder_tag {
public:
    using value_type = typename std::decay_t<InitialValue>;
    using decayed_folder = typename std::decay_t<Folder>;

    template<typename InitialValueForward, typename FolderForward>
    explicit FoldBinder(InitialValueForward&& initialValue, FolderForward&& folder)
        : initialValue_{std::forward<InitialValueForward>(initialValue)},
          folder_{std::forward<FolderForward>(folder)} {}

    FoldBinder(const FoldBinder&) = delete;

    FoldBinder(FoldBinder&&) = default;

    FoldBinder& operator=(const FoldBinder&) = delete;

    FoldBinder& operator=(FoldBinder&&) = default;

    template<typename InputRange>
    auto bind(InputRange&& inputRange) && {
        for (auto&& element : inputRange)
            initialValue_ = folder_(std::move(initialValue_), std::forward<decltype(element)>(element));
        return std::move(initialValue_);
    }

private:
    value_type initialValue_;
    decayed_folder folder_;
};

template<typename InitialValue, typename Folder>
auto fold(InitialValue&& initialValue, Folder&& folder) {
    return FoldBinder<InitialValue&&, Folder&&>(std::forward<InitialValue>(initialValue), std::forward<Folder>(folder));
}

}
