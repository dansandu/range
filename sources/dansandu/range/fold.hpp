#pragma once

#include "dansandu/range/category.hpp"

#include <type_traits>

namespace dansandu::range::fold {

template<typename InitialValue, typename Folder>
class FoldReductionFactory {
public:
    using range_factory_category = dansandu::range::category::reduction_factory_tag;
    using value_type = typename std::decay_t<InitialValue>;
    using folder_type = typename std::decay_t<Folder>;

    template<typename InitialValueForward, typename FolderForward>
    explicit FoldReductionFactory(InitialValueForward&& initialValue, FolderForward&& folder)
        : initialValue_{std::forward<InitialValueForward>(initialValue)},
          folder_{std::forward<FolderForward>(folder)} {}

    FoldReductionFactory(const FoldReductionFactory&) = delete;

    FoldReductionFactory(FoldReductionFactory&&) noexcept = default;

    FoldReductionFactory& operator=(const FoldReductionFactory&) = delete;

    FoldReductionFactory& operator=(FoldReductionFactory&&) noexcept = default;

    template<typename InputRange>
    auto create(InputRange&& inputRange) && {
        for (auto element : inputRange)
            initialValue_ = folder_(initialValue_, element);
        return std::move(initialValue_);
    }

private:
    value_type initialValue_;
    folder_type folder_;
};

template<typename InitialValue, typename Folder>
auto fold(InitialValue&& initialValue, Folder&& folder) {
    return FoldReductionFactory<InitialValue&&, Folder&&>(std::forward<InitialValue>(initialValue),
                                                          std::forward<Folder>(folder));
}

}
