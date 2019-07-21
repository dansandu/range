#include "catchorg/catch/catch.hpp"
#include "dansandu/range/category.hpp"

#include <vector>

using dansandu::range::category::container_tag;
using dansandu::range::category::generator_tag;
using dansandu::range::category::is_pipe_head;
using dansandu::range::category::is_range_factory;
using dansandu::range::category::reduction_factory_tag;
using dansandu::range::category::reduction_tag;
using dansandu::range::category::view_factory_tag;
using dansandu::range::category::view_tag;

struct ContainerMock {
    using range_category = container_tag;
};

struct GeneratorMock {
    using range_category = generator_tag;
};

struct ViewMock {
    using range_category = view_tag;
};

struct ReductionMock {
    using range_category = reduction_tag;
};

struct ViewFactoryMock {
    using range_factory_category = view_factory_tag;
};

struct ReductionFactoryMock {
    using range_factory_category = reduction_factory_tag;
};

TEST_CASE("Category") {
    SECTION("pipe head") {
        REQUIRE(is_pipe_head<std::vector<int>>);

        REQUIRE(is_pipe_head<ContainerMock>);

        REQUIRE(is_pipe_head<GeneratorMock>);

        REQUIRE(is_pipe_head<ViewMock>);

        REQUIRE(!is_pipe_head<ReductionMock>);

        REQUIRE(!is_pipe_head<ViewFactoryMock>);

        REQUIRE(!is_pipe_head<ReductionFactoryMock>);
    }

    SECTION("range factory") {
        REQUIRE(is_range_factory<ViewFactoryMock>);

        REQUIRE(is_range_factory<ReductionFactoryMock>);

        REQUIRE(!is_range_factory<ContainerMock>);

        REQUIRE(!is_range_factory<GeneratorMock>);

        REQUIRE(!is_range_factory<ViewMock>);

        REQUIRE(!is_range_factory<ReductionMock>);

        REQUIRE(!is_range_factory<std::vector<int>>);
    }
}
