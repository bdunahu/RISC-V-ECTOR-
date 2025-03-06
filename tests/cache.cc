#include "cache.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constructor initialize test 1", "[cache]")
{
	Cache *c = new Cache(1, nullptr, 4);
	std::array<signed int, 4> expected = {0, 0, 0, 0};
	std::array<signed int, 4> actual = c->view(0, 1)[0];
	CHECK(expected == actual);
	delete c;
}
