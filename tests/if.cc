#include "if.h"
#include "cache.h"
#include "controller.h"
#include "dram.h"
#include "instrDTO.h"
#include <catch2/catch_test_macros.hpp>

class IFPipeFixture
{
  public:
	IFPipeFixture()
	{
		Dram *d;

		d = new Dram(3);
		// 0xC00 is a nop
		p = {0xC000, 0xC001, 0xC002, 0xC003};
		d->load(p);

		this->c = new Cache(d, 1);
		this->f = new IF(nullptr);
		this->ct = new Controller(this->f, this->c, true);
	}
	~IFPipeFixture()
	{
		delete this->ct;
		delete this->c;
	};

	/**
	 * Fetch a clean line not in cache.
	 */
	void fetch_through(InstrDTO &instr)
	{
		int i;
		Response r;

		for (i = 0; i <= MEM_DELAY; ++i) {
			r = this->ct->advance(instr);
			// check response
			CHECK(r == BLOCKED);
		}
		this->fetch_cache(instr);
	}

	/**
	 * Fetch a line in cache.
	 */
	void fetch_cache(InstrDTO &instr)
	{
		int i;
		Response r;

		for (i = 0; i <= L1_CACHE_DELAY; ++i) {
			r = this->ct->advance(instr);
			// check response
			CHECK(r == WAIT);
		}
		r = this->ct->advance(instr);
		// check response
		CHECK(r == OK);
	}

	std::vector<signed int> p;
	Cache *c;
	IF *f;
	Controller *ct;
};

TEST_CASE_METHOD(IFPipeFixture, "fetch returns single instuction", "[if_pipe]")
{
	InstrDTO instr;
	int expected_cycles;

	expected_cycles = MEM_DELAY + L1_CACHE_DELAY + 2;
	this->fetch_through(instr);

	CHECK(instr.get_if_cycle() == expected_cycles);
	REQUIRE(instr.get_instr_bits() == this->p[0]);
}

TEST_CASE_METHOD(IFPipeFixture, "fetch returns two instuctions", "[if_pipe]")
{
	InstrDTO instr;
	int expected_cycles;

	expected_cycles = MEM_DELAY + L1_CACHE_DELAY + 2;
	this->fetch_through(instr);

	CHECK(instr.get_if_cycle() == expected_cycles);
	REQUIRE(instr.get_instr_bits() == this->p[0]);

	// is this right???
	expected_cycles += L1_CACHE_DELAY + 2;
	this->fetch_cache(instr);

	CHECK(instr.get_if_cycle() == expected_cycles);
	REQUIRE(instr.get_instr_bits() == this->p[1]);
}
