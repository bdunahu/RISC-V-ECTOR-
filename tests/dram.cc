#include "dram.h"
#include <catch2/catch_test_macros.hpp>
#include <array>

TEST_CASE("Constructor initialize test 1", "[dram]")
{
	Dram *d = new Dram(1, 4);
	std::array<signed int, 4> expected = {0, 0, 0, 0};
	std::array<signed int, 4> actual = d->view(0, 1)[0];
	CHECK(expected == actual);
	delete d;
}
