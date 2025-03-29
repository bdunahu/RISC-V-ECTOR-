#include "id.h"
#include "cache.h"
#include "controller.h"
#include "dram.h"
#include "if.h"
#include "instrDTO.h"
#include "instr.h"
#include <catch2/catch_test_macros.hpp>

class IDPipeFixture
{
  public:
	IDPipeFixture()
	{
		Dram *dr;

		dr = new Dram(this->m_delay);
		// 0xC00 is a nop
		p = {0xC000, 0xC001, 0xC002, 0xC003};
		dr->load(p);

		this->c = new Cache(dr, this->c_delay);
		IF *f = new IF(nullptr);
		this->d = new ID(f);
		this->ct = new Controller(this->d, this->c, true);
	};
	~IDPipeFixture()
	{
		delete this->ct;
		delete this->c;
	};

	// /**
	//  * Requests something to do until it is received.
	//  */
	// void fetch(InstrDTO &instr)
	// {
	// 	Response r = WAIT;

	// 	while (r != OK) {
	// 		r = this->ct->advance(instr);
	// 	}
	// }

	int m_delay = 3;
	int c_delay = 1;
	std::vector<signed int> p;
	Cache *c;
	ID *d;
	Controller *ct;
};

TEST_CASE_METHOD(IDPipeFixture, "Parse invalid type", "[id]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	Mnemonic m;

	s1 = 0x00FF00FF;
	this->d->get_instr_fields(s1, s2, s3, m);
	CHECK(m == NOP);
}

TEST_CASE_METHOD(IDPipeFixture, "Parse arbitrary r-type # one", "[id]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	Mnemonic m;

	s1 = 0xCCCCCC0C;
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0b11000);
	CHECK(s2 == 0b01100);
	CHECK(s3 == 0b00110);
	CHECK(m == MUL);
}

TEST_CASE_METHOD(IDPipeFixture, "Parse arbitrary r-type # two", "[id]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	Mnemonic m;

	s1 = 0x99AABB0C;
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0b10110);
	CHECK(s2 == 0b01011);
	CHECK(s3 == 0b10101);
	CHECK(m == MUL);
}

TEST_CASE_METHOD(IDPipeFixture, "Parse arbitrary i-type # one", "[id]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	Mnemonic m;

	s1 = 0xDDDDDDDD;
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0b10111);
	CHECK(s2 == 0b11011);
	CHECK(s3 == 0xDDDD);
	CHECK(m == SFTLI);
}

TEST_CASE_METHOD(IDPipeFixture, "Parse arbitrary i-type # two", "[id]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	Mnemonic m;

	s1 = 0xAABBCCDD;
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0b10011);
	CHECK(s2 == 0b11001);
	CHECK(s3 == 0xAABB);
	CHECK(m == SFTLI);
}

TEST_CASE_METHOD(IDPipeFixture, "Parse arbitrary j-type # one", "[id]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	Mnemonic m;

	s1 = 0xEEEEEE1E;
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0b11000);
	CHECK(s2 == 0b111011101110111011101);
	CHECK(m == BOF);
	// behavior does nothing
	CHECK(s3 == 0b0);
}

TEST_CASE_METHOD(IDPipeFixture, "Parse arbitrary j-type # two", "[id]")
{
	signed int s1 = 0, s2 = 0, s3 = 0;
	Mnemonic m;

	s1 = 0xBBCCDD0E;
	this->d->get_instr_fields(s1, s2, s3, m);

	CHECK(s1 == 0b10100);
	CHECK(s2 == 0b101110111100110011011);
	CHECK(m == JAL);
	// behavior does nothing
	CHECK(s3 == 0b0);
}
