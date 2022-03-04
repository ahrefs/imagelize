#include <catch2/catch_all.hpp>

#include <imagelize.hpp>

TEST_CASE("Initial test", "[initial]")
{
  REQUIRE(imagelize::initial() == true);
}