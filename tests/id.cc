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
		i->set_instr_bits(raw);
		this->dum->set_curr_instr(i);

		i = this->ct->advance(WAIT);
		REQUIRE(i == nullptr);
		i = this->ct->advance(WAIT);
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

	CHECK(i->get_mnemonic() == NOP);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary r-type # one", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_R_type(0b0, 0b1, 0b10, 0b11, 0b0);
	i = this->decode_bits(t);

	CHECK(i->get_s1() == 0x00000000); // registers are empty
	CHECK(i->get_s2() == 0x00000000);
	CHECK(i->get_s3() == 0x00000000);
	CHECK(i->get_mnemonic() == MUL);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary r-type # two", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_R_type(0b10000, 0b01000, 0b00100, 0b10, 0b0);
	i = this->decode_bits(t);

	CHECK(i->get_s1() == 0x00000000); // registers are empty
	CHECK(i->get_s2() == 0x00000000);
	CHECK(i->get_s3() == 0x00000000);
	CHECK(i->get_mnemonic() == SUB);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary i-type # one", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_I_type(0xF, 0b1, 0b10, 0b0111, 0b1);
	i = this->decode_bits(t);

	CHECK(i->get_s1() == 0x00000000); // registers are empty
	CHECK(i->get_s2() == 0x00000000);
	CHECK(i->get_s3() == 0xF);
	CHECK(i->get_mnemonic() == ANDI);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary i-type # two", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_I_type(0xCC, 0b010, 0b101, 0b1011, 0b1);
	i = this->decode_bits(t);

	CHECK(i->get_s1() == 0x00000000); // registers are empty
	CHECK(i->get_s2() == 0x00000000);
	CHECK(i->get_s3() == 0xCC);
	CHECK(i->get_mnemonic() == STOREV);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary j-type # one", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_J_type(0x3456, 0b10101, 0b0111, 0b10);
	i = this->decode_bits(t);

	CHECK(i->get_s1() == 0x00000000); // registers are empty
	CHECK(i->get_s2() == 0x3456);
	CHECK(i->get_mnemonic() == BOF);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary j-type # two", "[id]")
{
	signed int t;
	InstrDTO *i;

	t = this->encode_J_type(0xBBCCF, 0b10101, 0b0011, 0b10);
	i = this->decode_bits(t);

	CHECK(i->get_s1() == 0x00000000); // registers are empty
	CHECK(i->get_s2() == 0xFFFBBCCF);
	CHECK(i->get_mnemonic() == JAL);

	delete i;
}

TEST_CASE_METHOD(IDFixture, "read does not conflict with read", "[id]")
{
	signed int v;
	Response r;

	v = 0b1;
	r = this->d->read_guard(v);
	CHECK(v == 0b0);
	REQUIRE(r == OK);

	v = 0b1;
	this->d->read_guard(v);
	REQUIRE(v == 0b0);
}

TEST_CASE_METHOD(IDFixture, "write does not conflict with write", "[id]")
{
	signed int v;

	v = 0b1;
	this->d->write_guard(v);
	REQUIRE(v == 0b0);

	v = 0b1;
	this->d->write_guard(v);
	REQUIRE(v == 0b0);
}

TEST_CASE_METHOD(IDFixture, "write does not conflict with read", "[id]")
{
	signed int v;
	Response r;

	v = 0b1;
	r = this->d->read_guard(v);
	CHECK(v == 0b0);
	REQUIRE(r == OK);

	v = 0b1;
	this->d->write_guard(v);
	REQUIRE(v == 0b0);
}

TEST_CASE_METHOD(IDFixture, "read does conflict with write", "[id]")
{
	signed int v;
	Response r;

	v = 0b1;
	this->d->write_guard(v);
	REQUIRE(v == 0b0);

	v = 0b1;
	r = this->d->read_guard(v);
	CHECK(v == 0b01);
	REQUIRE(r == STALLED);
}

TEST_CASE_METHOD(IDFixture, "stores indefinite conflicts", "[id]")
{
	signed int v, ov;
	Response r;

	v = 0b1;
	ov = v;
	while (v < 0b110) {
		this->d->write_guard(v);
		REQUIRE(v == 0b0);
		v = ++ov;
	}
	this->d->write_guard(v);
	REQUIRE(v == 0b0);

	v = 0b110;
	r = this->d->read_guard(v);
	CHECK(v == 0b110);
	REQUIRE(r == STALLED);

	v = 0b1;
	r = this->d->read_guard(v);
	CHECK(v == 0b1);
	REQUIRE(r == STALLED);
}
