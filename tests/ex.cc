#include "ex.h"
#include "cache.h"
#include "controller.h"
#include "dram.h"
#include "dum.h"
#include "instr.h"
#include "instrDTO.h"
#include <catch2/catch_test_macros.hpp>

class EXFixture
{
  public:
	EXFixture()
	{
		this->dr = new Dram(3);
		this->c = new Cache(this->dr, 1);
		this->dum = new DUM(nullptr);
		this->e = new EX(dum);
		this->ct = new Controller(this->e, this->c, true);
	};
	~EXFixture()
	{
		delete this->ct;
		delete this->c;
	};
	InstrDTO *execute_instr(signed int s1, signed int s2, Mnemonic m)
	{
		InstrDTO *i = new InstrDTO();
		i->set_s1(s1);
		i->set_s2(s2);
		i->set_mnemonic(m);
		this->dum->set_curr_instr(i);

		i = this->ct->advance(OK);
		REQUIRE(i == nullptr);
		i = this->ct->advance(OK);
		REQUIRE(i != nullptr);

		return i;
	}
	Dram *dr;
	Cache *c;
	EX *e;
	DUM *dum;
	Controller *ct;
};

std::unordered_map<Mnemonic, signed int> results = {
	{ADD, 42},
	{SUB, 83958},
};

TEST_CASE_METHOD(EXFixture, "ADD within bounds", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = ADD;
	s1 = 42000, s2 = -41958;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == 42);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "ADD overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "ADD underflow", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "SUB within bounds", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = SUB;
	s1 = 200, s2 = 131;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == 69);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "SUB overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "SUB underflow", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "MUL within bounds", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = MUL;
	s1 = 200, s2 = 200;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == 40000);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "MUL overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "MUL underflow", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "QUOT within bounds", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = QUOT;
	s1 = 2043, s2 = 40;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == 51);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "QUOT overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "QUOT underflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "QUOT halt", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "REM within bounds", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = REM;
	s1 = 2043, s2 = 40;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == 3);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "REM overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "REM underflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "REM halt", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "SFTR within bounds", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = SFTR;
	s1 = 1300, s2 = 6;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == 20);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "SFTR overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "SFTR underflow", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "SFTL within bounds", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = SFTL;
	s1 = 13, s2 = 6;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == 832);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "SFTL overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "SFTL underflow", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "AND", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = AND;
	s1 = 1234, s2 = 5678;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == 1026);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "OR", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = OR;
	s1 = 1234, s2 = 5678;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == 5886);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "NOT", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = NOT;
	s1 = 1234, s2 = -1;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == -1235);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "XOR", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = XOR;
	s1 = 1234, s2 = 5678;
	i = execute_instr(s1, s2, m);

	CHECK(i->get_s1() == 4860);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "ADDV within bounds", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "ADDV overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "ADDV underflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "SUBV within bounds", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "SUBV overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "SUBV underflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "MULV within bounds", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "MULV overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "MULV underflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "DIVV within bounds", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "DIVV overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "DIVV underflow", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "CMP less", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = CMP;
	s1 = -1, s2 = 0;
	i = execute_instr(s1, s2, m);

	// should not be changed
	CHECK(i->get_s1() == -1);

	CHECK(!ct->get_condition(EQ));
	CHECK(!ct->get_condition(GT));
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "CMP equal", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = CMP;
	s1 = 20, s2 = 20;
	i = execute_instr(s1, s2, m);

	// should not be changed
	CHECK(i->get_s1() == 20);

	CHECK(ct->get_condition(EQ));
	CHECK(!ct->get_condition(GT));
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "CMP greater", "[ex]")
{
	signed int s1 = 0, s2 = 0;
	Mnemonic m;
	InstrDTO *i;

	m = CMP;
	s1 = 21, s2 = 20;
	i = execute_instr(s1, s2, m);

	// should not be changed
	CHECK(i->get_s1() == 21);

	CHECK(!ct->get_condition(EQ));
	CHECK(ct->get_condition(GT));
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "CEQ less", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "CEQ equal", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "CEQ greater", "[ex]")
// {
// }
