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
		MM *m = new MM(e);
		WB *w = new WB(m);
		this->stage_num = 5;

		this->ct = new Controller(w, this->c, true);
	}
	~ControllerPipeFixture()
	{
		delete this->ct;
		delete this->c;
	};
	void fill_pipe()
	{
		InstrDTO *i = nullptr;
		int j;
		for (j = 0; j < this->stage_num + 1; ++j) {
			i = this->ct->advance(WAIT);
			REQUIRE(i == nullptr);
		}
	}
	int stage_num;
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

TEST_CASE_METHOD(ControllerPipeFixture, "two num adder", "[full pipe]")
{
	signed int b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11;
	std::vector<signed int> p;
	InstrDTO *i;

	// I-TYPE              /    /    /   /
	b0 = 0b00000010000000000001000000001101; // ADDI $sp $0 0x200;
	b1 = 0b00000000000000010010100000001101; // ADDI $5 $0 0x1;
	b2 = 0b00000000000000000001000101101101; // STORE $5 0($sp);  (RAW HAZARD)!
	// I-TYPE              /    /    /   /
	b3 = 0b00000000000000100010100000001101; // ADDI $5 $0 0x2;   (RAW HAZARD)!
	b4 = 0b00000000000000010001000101101101; // STORE $5 1($sp);  (RAW HAZARD)!
	// // I-TYPE              /    /    /   /
	// b9 = 0b00000000000000000010100000001101;  // ADDI $5 $0 0x0;
	// // I-TYPE               /    /    /   /
	// b10 = 0b00000000000000110011000000001101; // ADDI $6 $0 0x3;
	// // J-TYPE                    /    /   /
	// b11 = 0b00000000000000000011100000001010; // jrl CHECK

	p = {b0, b1, b2, b3};
	this->d->load(p);

	this->fill_pipe();
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 3);
	CHECK(i->get_time_of(DCDE) == 4);
	CHECK(i->get_time_of(EXEC) == 5);
	CHECK(i->get_time_of(MEM) == 6);
	CHECK(i->get_time_of(WRITE) == 7);
	CHECK(i->get_s1() == 0x200);
	CHECK(i->get_s2() == 0x0);
	CHECK(i->get_s3() == 0x200);
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(i->get_mnemonic() == ADDI);
	CHECK(i->get_instr_bits() == b0);

	delete i;
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 4);
	CHECK(i->get_time_of(DCDE) == 5);
	CHECK(i->get_time_of(EXEC) == 6);
	CHECK(i->get_time_of(MEM) == 7);
	CHECK(i->get_time_of(WRITE) == 8);
	CHECK(i->get_s1() == 0x1);
	CHECK(i->get_s2() == 0x0);
	CHECK(i->get_s3() == 0x1);
	CHECK(this->ct->get_gprs().at(5) == 0x1);
	CHECK(i->get_mnemonic() == ADDI);
	CHECK(i->get_instr_bits() == b1);

	delete i;
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 5);
	CHECK(i->get_time_of(DCDE) == 8); // the previous conflicting instruction wrote here!
	CHECK(i->get_time_of(EXEC) == 9);
	CHECK(i->get_time_of(MEM) == 12); // waited for fetch + 2 dram
	CHECK(i->get_time_of(WRITE) == 13);
	CHECK(i->get_s1() == 0x1);
	CHECK(i->get_s2() == 0x200);
	CHECK(i->get_s3() == 0x0);
	// NO STORE
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(5) == 0x1);
	CHECK(i->get_mnemonic() == STORE);
	CHECK(i->get_instr_bits() == b2);

	delete i;
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 8);
	CHECK(i->get_time_of(DCDE) == 9);
	CHECK(i->get_time_of(EXEC) == 12);
	CHECK(i->get_time_of(MEM) == 13);
	CHECK(i->get_time_of(WRITE) == 14);
	CHECK(i->get_s1() == 0x2);
	CHECK(i->get_s2() == 0x0);
	CHECK(i->get_s3() == 0x2);
	// NO STORE
	CHECK(this->ct->get_gprs().at(5) == 0x2);
	CHECK(i->get_mnemonic() == ADDI);
	CHECK(i->get_instr_bits() == b3);

	delete i;
}
