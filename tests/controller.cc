#include "controller.h"
#include "cache.h"
#include "dram.h"
#include "ex.h"
#include "id.h"
#include "if.h"
#include "mm.h"
#include "wb.h"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <vector>

class ControllerPipeFixture
{
  public:
	ControllerPipeFixture()
	{
		this->d = new Dram(1);
		this->c = new Cache(this->d, 0);

		IF *f = new IF(nullptr);
		ID *d = new ID(f);
		EX *e = new EX(d);

		this->ct = new Controller(e, this->c, true);
	}
	~ControllerPipeFixture()
	{
		delete this->ct;
		delete this->c;
	};

	Cache *c;
	Dram *d;
	Controller *ct;
};

TEST_CASE_METHOD(
	ControllerPipeFixture,
	"Contructor resets gettable fields",
	"[controller_pipe]")
{
	std::array<int, GPR_NUM> gprs;

	gprs = this->ct->get_gprs();

	CHECK(this->ct->get_clock_cycle() == 1);
	CHECK(std::all_of(
		gprs.begin(), gprs.end(), [](int value) { return value == 0; }));
	// change me later
	CHECK(this->ct->get_pc() == 0);
}

TEST_CASE_METHOD(ControllerPipeFixture, "Add until exec", "[tmp]")
{
	signed int b;
	std::vector<signed int> p;
	InstrDTO *i;

	b = 0b1010100000000001001101;
	p = {b};
	this->d->load(p);

	// dram
	i = this->ct->advance(OK);
	REQUIRE(i == nullptr);
	// fetch
	i = this->ct->advance(OK);
	REQUIRE(i == nullptr);
	// decode
	i = this->ct->advance(OK);
	REQUIRE(i == nullptr);
	// exec
	i = this->ct->advance(OK);
	REQUIRE(i == nullptr);
	// done
	i = this->ct->advance(OK);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 3);
	CHECK(i->get_time_of(DCDE) == 4);
	CHECK(i->get_time_of(EXEC) == 5);
	CHECK(i->get_s1() == 42);
	CHECK(i->get_s2() == 0);
	CHECK(i->get_s3() == 42);
	CHECK(i->get_mnemonic() == ADDI);
	CHECK(i->get_instr_bits() == b);

	delete i;
}
