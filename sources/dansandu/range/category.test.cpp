#include "catchorg/catch/catch.hpp"
#include "dansandu/range/category.hpp"

#include <map>
#include <string>
#include <vector>

using dansandu::range::category::is_pipe_head;
using dansandu::range::category::is_range_binder;
using dansandu::range::category::range_binder_tag;

struct RangeBinderMock : range_binder_tag
{
};

struct NotArange
{
    auto cbegin() const
    {
        return values.cbegin();
    }

    auto cend() const
    {
        return values.cbegin();
    }

    std::vector<int> values;
};

TEST_CASE("Category")
{
    SECTION("pipe head")
    {
        REQUIRE(is_pipe_head<std::vector<int>>);

        REQUIRE(is_pipe_head<std::map<std::string, std::vector<double>>>);

        REQUIRE(is_pipe_head<std::string>);

        REQUIRE(!is_pipe_head<NotArange>);

        REQUIRE(!is_pipe_head<RangeBinderMock>);

        REQUIRE(!is_pipe_head<double>);
    }

    SECTION("range binder")
    {
        REQUIRE(is_range_binder<RangeBinderMock>);

        REQUIRE(!is_range_binder<std::vector<const char*>>);

        REQUIRE(!is_range_binder<const char*>);
    }
}
