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
	signed int b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14,
		b15;
	std::vector<signed int> p;
	InstrDTO *i;

	// I-TYPE              /    /    /   /
	b0 = 0b00000010000000000001000000001101; // ADDI $2 $0 0x200;
	b1 = 0b00000000000000010010100000001101; // ADDI $5 $0 0x1;
	b2 = 0b00000000000000000010100010101101; // STORE $5 0($2);  (RAW HAZARD w
											 // 1)!
	// I-TYPE              /    /    /   /
	b3 = 0b00000000000000100010100000001101; // ADDI $5 $0 0x2;   (RAW HAZARD)!
	b4 = 0b00000000000000010010100010101101; // STORE $5 1($2);  (RAW HAZARD)!
	// // I-TYPE           /    /    /   /
	b5 = 0b00000000000000000010100000001101; // ADDI $5 $0 0x0;
	// // I-TYPE           /    /    /   /
	b6 = 0b00000000000000010011000000001101; // ADDI $6 $0 0x1;
	// // J-TYPE                /    /   /
	b7 = 0b00000000000000000011100000001010; // JRL CHECK
	// // R-TYPE     /    /    /    /    /
	b8 = 0b00000000000100100101000100000100; // ADD $9 $2 $5;
	// // I-TYPE           /    /    /   /
	b9 = 0b00000000000000000011101001000101; // LOAD $7 0($9); (RAW HAZARD)!
	//  // I-TYPE           /    /    /   /
	b10 = 0b00000000000000010100001001000101; // LOAD $8 1($9);
	//  // R-TYPE     /    /    /    /    /
	b11 = 0b00000000000011101000001110000100; // ADD $7 $7 $8;
	// I-TYPE               /    /    /   /
	b12 = 0b00000000000000000011101001101101; // STORE $7 0($9);
	b13 = 0b00000010000000010010100101001101; // ADDI $5 $5 0x1;
	//  // R-TYPE     /    /    /    /    /
	b14 = 0b00000000000111100101001101000000; // CMP $6 $5
	//  // J-TYPE                /    /   /
	b15 = 0b11111111111111111100100000010110; // bgt LOOP

	p = {b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15};
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
	CHECK(this->ct->checked_out.front() == 0x5);

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
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 5);
	CHECK(
		i->get_time_of(DCDE) ==
		8); // the previous conflicting instruction wrote here!
	CHECK(i->get_time_of(EXEC) == 9);
	CHECK(i->get_time_of(MEM) == 14); // waited for fetch + 3 dram
	CHECK(i->get_time_of(WRITE) == 15);
	CHECK(i->get_s1() == 0x200);
	CHECK(i->get_s2() == 0x1);
	CHECK(i->get_s3() == 0x0);
	// NO STORE
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(5) == 0x1);
	CHECK(i->get_mnemonic() == STORE);
	CHECK(i->get_instr_bits() == b2);
	CHECK(this->ct->checked_out.front() == 0x5);

	delete i;
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 8);
	CHECK(i->get_time_of(DCDE) == 9);
	CHECK(i->get_time_of(EXEC) == 14);
	CHECK(i->get_time_of(MEM) == 15);
	CHECK(i->get_time_of(WRITE) == 16);
	CHECK(i->get_s1() == 0x2);
	CHECK(i->get_s2() == 0x1); // the previous value in the destination register
	CHECK(i->get_s3() == 0x2);
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(5) == 0x2);
	CHECK(i->get_mnemonic() == ADDI);
	CHECK(i->get_instr_bits() == b3);

	delete i;

	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 14); // fetching new line + mem
	CHECK(
		i->get_time_of(DCDE) ==
		16); // the previous conflicting instruction wrote here!
	CHECK(i->get_time_of(EXEC) == 17);
	CHECK(i->get_time_of(MEM) == 18);
	CHECK(i->get_time_of(WRITE) == 19);
	CHECK(i->get_s1() == 0x201);
	CHECK(i->get_s2() == 0x2); // address
	CHECK(i->get_s3() == 0x1); // offset
	// NO STORE
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(5) == 0x2);
	CHECK(i->get_mnemonic() == STORE);
	CHECK(i->get_instr_bits() == b4);
	// CHECK(this->ct->checked_out.front() == 0x5);

	delete i;
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 16);
	CHECK(i->get_time_of(DCDE) == 17);
	CHECK(i->get_time_of(EXEC) == 18);
	CHECK(i->get_time_of(MEM) == 19);
	CHECK(i->get_time_of(WRITE) == 20);
	CHECK(i->get_s1() == 0x0);
	CHECK(i->get_s2() == 0x2);
	CHECK(i->get_s3() == 0x0);
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(5) == 0x0);
	CHECK(i->get_mnemonic() == ADDI);
	CHECK(i->get_instr_bits() == b5);

	delete i;
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 17);
	CHECK(i->get_time_of(DCDE) == 18);
	CHECK(i->get_time_of(EXEC) == 19);
	CHECK(i->get_time_of(MEM) == 20);
	CHECK(i->get_time_of(WRITE) == 21);
	CHECK(i->get_s1() == 0x1);
	CHECK(i->get_s2() == 0x0);
	CHECK(i->get_s3() == 0x1);
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(6) == 0x1);
	CHECK(i->get_pc() == 0x6);
	CHECK(this->ct->get_pc() == 0x9);
	CHECK(i->get_mnemonic() == ADDI);
	CHECK(i->get_instr_bits() == b6);

	delete i;
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 18);
	CHECK(i->get_time_of(DCDE) == 19);
	CHECK(i->get_time_of(EXEC) == 20);
	CHECK(i->get_time_of(MEM) == 21);
	CHECK(i->get_time_of(WRITE) == 22);
	CHECK(i->get_s1() == 0xE);
	CHECK(i->get_s2() == 0x7);
	CHECK(i->get_s3() == 0x0); // doesn't exist
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(6) == 0x1);
	CHECK(i->get_pc() == 0x7);
	CHECK(this->ct->get_pc() == 0xE);
	CHECK(i->get_mnemonic() == JRL);
	CHECK(i->get_instr_bits() == b7);

	delete i;
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr); // switching cache lines in fetch
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_mnemonic() == NOP); // squashed

	delete i;
	i = this->ct->advance(WAIT); // nops?
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(
		i->get_time_of(FETCH) ==
		24); // 6 greater than last fetch (4 flush pipe, 2 dram)
	CHECK(i->get_time_of(DCDE) == 25);
	CHECK(i->get_time_of(EXEC) == 26);
	CHECK(i->get_time_of(MEM) == 27);
	CHECK(i->get_time_of(WRITE) == 28);
	CHECK(i->get_s1() == 0x1);
	CHECK(i->get_s2() == 0x0);
	CHECK(i->get_s3() == 0x0);
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(5) == 0x0);
	CHECK(this->ct->get_gprs().at(6) == 0x1);
	CHECK(this->ct->get_condition(GT) == true);
	CHECK(this->ct->get_condition(EQ) == false);
	CHECK(i->get_mnemonic() == CMP);
	CHECK(i->get_instr_bits() == b14);

	delete i;
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 25);
	CHECK(i->get_time_of(DCDE) == 26);
	CHECK(i->get_time_of(EXEC) == 27);
	CHECK(i->get_time_of(MEM) == 28);
	CHECK(i->get_time_of(WRITE) == 29);
	CHECK(i->get_s1() == 0x8);
	CHECK(i->get_s2() == 0b111111111111111111001);
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(5) == 0x0);
	CHECK(this->ct->get_gprs().at(6) == 0x1);
	CHECK(this->ct->get_condition(GT) == true);
	CHECK(this->ct->get_condition(EQ) == false);
	CHECK(this->ct->get_pc() == 0x9);
	CHECK(i->get_mnemonic() == BGT);
	CHECK(i->get_instr_bits() == b15);

	delete i;
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_mnemonic() == NOP);
	delete i;
	
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr); // it was already in cache
	
	CHECK(i->get_time_of(FETCH) == 29); // clear out pipe (4)
	CHECK(i->get_time_of(DCDE) == 30);
	CHECK(i->get_time_of(EXEC) == 31);
	CHECK(i->get_time_of(MEM) == 32);
	CHECK(i->get_time_of(WRITE) == 33);
	CHECK(i->get_s1() == 0x200);
	CHECK(i->get_s2() == 0x0);
	CHECK(i->get_s3() == 0x0);
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(6) == 0x1);
	CHECK(this->ct->get_gprs().at(9) == 0x200);
	CHECK(i->get_pc() == 0x8);
	CHECK(this->ct->get_pc() == 0xB);
	CHECK(i->get_mnemonic() == ADD);
	CHECK(i->get_instr_bits() == b8);

	delete i;
	i = this->ct->advance(WAIT); // RAW
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i == nullptr);
	i = this->ct->advance(WAIT);
	REQUIRE(i != nullptr);

	CHECK(i->get_time_of(FETCH) == 30);
	CHECK(i->get_time_of(DCDE) == 33);
	CHECK(i->get_time_of(EXEC) == 34);
	CHECK(i->get_time_of(MEM) == 35);
	CHECK(i->get_time_of(WRITE) == 36);
	CHECK(i->get_s1() == 0x0);
	CHECK(i->get_s2() == 0x200);
	CHECK(i->get_s3() == 0x0);
	CHECK(this->ct->get_gprs().at(2) == 0x200);
	CHECK(this->ct->get_gprs().at(6) == 0x1);
	CHECK(this->ct->get_gprs().at(7) == 0x1);
	CHECK(this->ct->get_gprs().at(9) == 0x200);
	CHECK(i->get_mnemonic() == LOAD);
	CHECK(i->get_instr_bits() == b9);

	delete i;
}
