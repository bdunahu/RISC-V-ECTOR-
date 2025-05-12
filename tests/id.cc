#include "id.h"
#include "cache.h"
#include "controller.h"
#include "dram.h"
#include "dum.h"
#include "instr.h"
#include "instrDTO.h"
#include <catch2/catch_test_macros.hpp>

class IDFixture
{
  public:
	IDFixture()
	{
		this->dr = new Dram(3);
		this->c = new Cache(this->dr, 5, 0, 1);
		this->dum = new DUM(nullptr);
		this->d = new ID(dum);
		this->ct = new Controller(this->d, this->c, true);
	};
	~IDFixture()
	{
		delete this->ct;
		delete this->c;
	};
	InstrDTO *decode_bits(signed int raw)
	{
		InstrDTO *i = new InstrDTO();
		i->slot_A = raw;
		this->dum->set_curr_instr(i);

		i = this->ct->advance(READY);
		REQUIRE(i == nullptr);
		i = this->ct->advance(READY);
		REQUIRE(i != nullptr);

		return i;
	}
	signed int encode_R_type(
		signed int s3,
		signed int s2,
		signed int s1,
		signed int opcode,
		signed int type)
	{
		signed int t;
		t = s3;
		t = (t << REG_SIZE) + s2;
		t = (t << REG_SIZE) + s1;
		t = (t << R_OPCODE_SIZE) + opcode;
		t = (t << TYPE_SIZE) + type;
		return t;
	}
	signed int encode_I_type(
		signed int s3,
		signed int s2,
		signed int s1,
		signed int opcode,
		signed int type)
	{
		signed int t;
		t = s3;
		t = (t << REG_SIZE) + s2;
		t = (t << REG_SIZE) + s1;
		t = (t << OPCODE_SIZE) + opcode;
		t = (t << TYPE_SIZE) + type;
		return t;
	}
	signed int encode_J_type(
		signed int s2, signed int s1, signed int opcode, signed int type)
	{
		signed int t;
		t = s2;
		t = (t << REG_SIZE) + s1;
		t = (t << OPCODE_SIZE) + opcode;
		t = (t << TYPE_SIZE) + type;
		return t;
	}

	Dram *dr;
	Cache *c;
	ID *d;
	DUM *dum;
	Controller *ct;
};

TEST_CASE_METHOD(IDFixture, "Parse invalid type", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_R_type(0b0, 0b1, 0b10, 0b11, 0b11);
	i = this->decode_bits(t);

	CHECK(i->mnemonic == NOP);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary r-type # one", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_R_type(0b101, 0b110, 0b111, 0b11, 0b0);
	i = this->decode_bits(t);

	CHECK(i->operands.integer.slot_one == 0x00000000); // registers are empty
	CHECK(i->operands.integer.slot_two == 0x00000000);
	CHECK(i->operands.integer.slot_three == 0x00000000);
	CHECK(i->mnemonic == MUL);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary r-type # two", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_R_type(0b1000, 0b01000, 0b00100, 0b10, 0b0);
	i = this->decode_bits(t);

	CHECK(i->operands.integer.slot_one == 0x00000000); // registers are empty
	CHECK(i->operands.integer.slot_two == 0x00000000);
	CHECK(i->operands.integer.slot_three == 0x00000000);
	CHECK(i->mnemonic == SUB);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary r-type # three", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_R_type(0b10000, 0b10001, 0b10101, 0b1110, 0b0);
	i = this->decode_bits(t);

	CHECK(i->operands.integer.slot_one == 0x00000000); // registers are empty
	CHECK(i->operands.integer.slot_two == 0x00000000);
	CHECK(i->operands.integer.slot_three == 0x00000000);
	CHECK(i->mnemonic == MULV);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary i-type # one", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_I_type(0xF, 0b101, 0b110, 0b0111, 0b1);
	i = this->decode_bits(t);

	CHECK(i->operands.integer.slot_one == 0x00000000); // registers are empty
	CHECK(i->operands.integer.slot_two == 0x00000000);
	CHECK(i->operands.integer.slot_three == 0xF);
	CHECK(i->mnemonic == ANDI);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary i-type # two", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_I_type(0xCC, 0b10101, 0b00110, 0b11010, 0b1);
	i = this->decode_bits(t);

	CHECK(i->operands.integer.slot_one == 0x00000000); // registers are empty
	CHECK(i->operands.integer.slot_two == 0x00000000);
	CHECK(i->operands.integer.slot_three == 0xCC);
	CHECK(i->mnemonic == STORE);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary j-type # one", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_J_type(0x3456, 0b101, 0b0111, 0b10);
	i = this->decode_bits(t);

	CHECK(i->operands.integer.slot_one == 0x00000000); // registers are empty
	CHECK(i->operands.integer.slot_two == 0x3456);
	CHECK(i->mnemonic == BOF);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary j-type # two", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_J_type(0xBBCCF, 0b00101, 0b0011, 0b10);
	i = this->decode_bits(t);

	t = 0xFFFBBCCF;
	CHECK(i->operands.integer.slot_one == 0x00000000); // registers are empty
	CHECK(i->operands.integer.slot_two == t);
	CHECK(i->mnemonic == JAL);

	delete i;
}
