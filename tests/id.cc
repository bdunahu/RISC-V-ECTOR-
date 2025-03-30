#include "id.h"
#include "cache.h"
#include "controller.h"
#include "dram.h"
#include "if.h"
#include "instr.h"
#include "instrDTO.h"
#include <catch2/catch_test_macros.hpp>

class IDFixture
{
  public:
	IDFixture()
	{
		Dram *dr;

		dr = new Dram(3);
		this->c = new Cache(dr, 1);
		IF *f = new IF(nullptr);
		this->d = new ID(f);
		this->ct = new Controller(this->d, this->c, true);
	};
	~IDFixture()
	{
		delete this->ct;
		delete this->c;
	};
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
	Cache *c;
	ID *d;
	Controller *ct;
};

TEST_CASE_METHOD(IDFixture, "Parse invalid type", "[id]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	Mnemonic m;

	s1 = 0xFFFFFFFF;
	this->d->get_instr_fields(s1, s2, s3, m);
	CHECK(m == NOP);
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary r-type # one", "[id]")
{
	signed int s1 = -1, s2 = -1, s3 = -1;
	Mnemonic m;

	s1 = this->encode_R_type(0b0, 0b1, 0b10, 0b11, 0b0);
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0x00000000); // registers are empty
	CHECK(s2 == 0x00000000);
	CHECK(s3 == 0x00000000);
	CHECK(m == MUL);
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary r-type # two", "[id]")
{
	signed int s1 = -1, s2 = -1, s3 = -1;
	Mnemonic m;

	s1 = this->encode_R_type(0b10000, 0b01000, 0b00100, 0b10, 0b0);
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0x00000000); // registers are empty
	CHECK(s2 == 0b00000000);
	CHECK(s3 == 0b00000000);
	CHECK(m == SUB);
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary i-type # one", "[id]")
{
	signed int s1 = -1, s2 = -1, s3 = -1;
	Mnemonic m;

	s1 = this->encode_I_type(0xF, 0b1, 0b10, 0b0111, 0b1);
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0x00000000); // registers are empty
	CHECK(s2 == 0x00000000);
	CHECK(s3 == 0xF);
	CHECK(m == SFTLI);
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary i-type # two", "[id]")
{
	signed int s1 = -1, s2 = -1, s3 = -1;
	Mnemonic m;

	s1 = this->encode_I_type(0xCC, 0b010, 0b101, 0b1011, 0b1);
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0x00000000); // registers are empty
	CHECK(s2 == 0x00000000);
	CHECK(s3 == 0xCC);
	CHECK(m == STORE);
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary j-type # one", "[id]")
{
	signed int s1 = -1, s2 = -1, s3 = -1;
	Mnemonic m;

	s1 = this->encode_J_type(0x3456, 0b10101, 0b0111, 0b10);
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0x00000000); // registers are empty
	CHECK(s2 == 0x3456);
	CHECK(m == BOF);
	// behavior does nothing
	CHECK(s3 == -1);
}

TEST_CASE_METHOD(IDFixture, "Parse arbitrary j-type # two", "[id]")
{
	signed int s1 = -1, s2 = -1, s3 = -1;
	Mnemonic m;

	s1 = this->encode_J_type(0xBBCCF, 0b10101, 0b0011, 0b10);
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0x00000000); // registers are empty
	CHECK(s2 == 0xBBCCF);
	CHECK(m == JAL);
	// behavior does nothing
	CHECK(s3 == -1);
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
	REQUIRE(r == BLOCKED);
}

TEST_CASE_METHOD(IDFixture, "stores indefinite conflicts", "[id]")
{
	signed int v, ov;
	Response r;

	v = 0b0;
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
	REQUIRE(r == BLOCKED);

	v = 0b0;
	r = this->d->read_guard(v);
	CHECK(v == 0b0);
	REQUIRE(r == BLOCKED);
}
