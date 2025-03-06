#include "dram.h"
#include "definitions.h"
#include <array>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Construct singleton dram", "[dram]")
{
	Dram *d = new Dram(1, 1);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	delete d;
}

TEST_CASE(
	"Construct singleton dram, store 0th element in zero cycles", "[dram]")
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

TEST_CASE(
	"Construct singleton dram, store 0th element in three cycles", "[dram]")
{
	Dram *d = new Dram(1, 3);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w = 0x11223344;

	// MEMORY CYCLE 1
	Response *r = d->write(MEMORY, w, 0x00000000);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	// MEMORY CYCLE 2
	r = d->write(MEMORY, w, 0x00000000);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	// MEMORY CYCLE 3
	r = d->write(MEMORY, w, 0x00000000);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	// MEMORY CYCLE 4
	r = d->write(MEMORY, w, 0x00000000);
	REQUIRE(r->status == OK);
	delete r;

	expected.at(0) = w;
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);

	delete d;
}

TEST_CASE(
	"Construct singleton dram, store 0, 1th element in three cycles no "
	"conflict",
	"[dram]")
{
	Dram *d = new Dram(1, 3);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w1 = 0x11223344;
	signed int w2 = 0x55667788;

	// MEMORY CYCLE 1
	Response *r = d->write(MEMORY, w1, 0x00000000);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	// MEMORY CYCLE 2
	actual = d->view(0, 1)[0];
	r = d->write(MEMORY, w1, 0x00000000);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	// MEMORY CYCLE 3
	r = d->write(MEMORY, w1, 0x00000000);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	// MEMORY CYCLE 4
	r = d->write(MEMORY, w1, 0x00000000);
	REQUIRE(r->status == OK);
	delete r;
	// NOTE: servicing on the same clock cycle should probably not be allowed
	// FETCH CYCLE 1
	r = d->write(FETCH, w2, 0x00000001);
	actual = d->view(0, 1)[0];
	REQUIRE(r->status == WAIT);
	delete r;

	expected.at(0) = w1;
	actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	// FETCH CYCLE 2
	r = d->write(FETCH, w2, 0x00000001);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	// FETCH CYCLE 3
	r = d->write(FETCH, w2, 0x00000001);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	// FETCH CYCLE 4
	r = d->write(FETCH, w2, 0x00000001);
	actual = d->view(0, 1)[0];
	REQUIRE(r->status == OK);
	delete r;

	expected.at(1) = w2;
	actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	delete d;
}

TEST_CASE(
	"Construct singleton dram, store 0, 1th element in three cycles much "
	"conflict",
	"[dram]")
{
	Dram *d = new Dram(1, 3);
	std::array<signed int, LINE_SIZE> expected = {0, 0, 0, 0};
	std::array<signed int, LINE_SIZE> actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	signed int w1 = 0x11223344;
	signed int w2 = 0x55667788;

	// MEMORY CYCLE 1
	Response *r = d->write(MEMORY, w1, 0x00000000);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	// MEMORY CYCLE 2
	actual = d->view(0, 1)[0];
	r = d->write(MEMORY, w1, 0x00000000);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;
	// FETCH CYCLE 1
	r = d->write(FETCH, w2, 0x00000001);
	actual = d->view(0, 1)[0];
	REQUIRE(r->status == WAIT);
	delete r;

	r = d->write(MEMORY, w1, 0x00000000);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;
	// FETCH CYCLE 1
	r = d->write(FETCH, w2, 0x00000001);
	actual = d->view(0, 1)[0];
	REQUIRE(r->status == WAIT);
	delete r;

	r = d->write(MEMORY, w1, 0x00000000);
	REQUIRE(r->status == OK);
	delete r;
	// NOTE: servicing on the same clock cycle should probably not be allowed
	// FETCH CYCLE 1
	r = d->write(FETCH, w2, 0x00000001);
	actual = d->view(0, 1)[0];
	REQUIRE(r->status == WAIT);
	delete r;

	expected.at(0) = w1;
	actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	r = d->write(FETCH, w2, 0x00000001);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	r = d->write(FETCH, w2, 0x00000001);
	actual = d->view(0, 1)[0];
	REQUIRE(expected == actual);
	REQUIRE(r->status == WAIT);
	delete r;

	r = d->write(FETCH, w2, 0x00000001);
	actual = d->view(0, 1)[0];
	REQUIRE(r->status == OK);
	delete r;

	expected.at(1) = w2;
	actual = d->view(0, 1)[0];
	CHECK(expected == actual);

	delete d;
}
