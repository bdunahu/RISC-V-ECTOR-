#include "dram.h"
#include "definitions.h"
#include <catch2/catch_test_macros.hpp>
#include <array>

TEST_CASE("Construct singleton dram", "[dram]")
{
	Dram *d = new Dram(1, 1);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	delete d;
}

TEST_CASE("Construct ingleton dram, store 0th element in zero cycles", "[dram]")
{
	Dram *d = new Dram(1, 0);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	Response *r = d->write(MEMORY, w, 0x00000000);
	REQUIRE(r->status == OK);


	expected.at(0) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete r;
	delete d;
}
