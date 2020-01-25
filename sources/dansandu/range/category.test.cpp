#include "catchorg/catch/catch.hpp"
#include "dansandu/range/category.hpp"

#include <vector>

using dansandu::range::category::container_tag;
using dansandu::range::category::generator_tag;
using dansandu::range::category::is_pipe_head;
using dansandu::range::category::is_range_binder;
using dansandu::range::category::range_binder_tag;
using dansandu::range::category::reduction_tag;
using dansandu::range::category::view_tag;

struct ContainerMock
{
    using range_category = container_tag;
};

struct GeneratorMock
{
    using range_category = generator_tag;
};

struct ViewMock
{
    using range_category = view_tag;
};

struct ReductionMock
{
    using range_category = reduction_tag;
};

struct RangeBinderMock : range_binder_tag
{
};

TEST_CASE("Category")
{
    SECTION("pipe head")
    {
        REQUIRE(is_pipe_head<std::vector<int>>);

        REQUIRE(is_pipe_head<ContainerMock>);

        REQUIRE(is_pipe_head<GeneratorMock>);

        REQUIRE(is_pipe_head<ViewMock>);

        REQUIRE(is_pipe_head<std::vector<double>>);

        REQUIRE(!is_pipe_head<ReductionMock>);

        REQUIRE(!is_pipe_head<RangeBinderMock>);

        REQUIRE(!is_pipe_head<double>);
    }

    SECTION("range binder")
    {
        REQUIRE(is_range_binder<RangeBinderMock>);

        REQUIRE(!is_range_binder<ContainerMock>);

        REQUIRE(!is_range_binder<GeneratorMock>);

        REQUIRE(!is_range_binder<ViewMock>);

        REQUIRE(!is_range_binder<ReductionMock>);

        REQUIRE(!is_range_binder<std::vector<const char*>>);

        REQUIRE(!is_range_binder<const char*>);
    }
}
