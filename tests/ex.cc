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
		this->c = new Cache(this->dr, 5, 0, 1);
		this->dum = new DUM(nullptr);
		this->e = new EX(dum);
		this->ct = new Controller(this->e, this->c, true);
	};
	~EXFixture()
	{
		delete this->ct;
		delete this->c;
	};
	InstrDTO *
	execute_instr(signed int s1, signed int s2, signed int s3, Mnemonic m)
	{
		InstrDTO *i = new InstrDTO();
		i->operands.integer.slot_one = s1;
		i->operands.integer.slot_two = s2;
		i->operands.integer.slot_three = s3;
		i->mnemonic = m;
		this->dum->set_curr_instr(i);

		i = this->ct->advance(READY);
		REQUIRE(i == nullptr);
		i = this->ct->advance(READY);
		REQUIRE(i != nullptr);

		return i;
	}
	Dram *dr;
	Cache *c;
	EX *e;
	DUM *dum;
	Controller *ct;
};

TEST_CASE_METHOD(EXFixture, "ADD within bounds", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = ADD;
	s1 = 42000, s2 = -41958, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 42);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "ADD overflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = ADD;
	s1 = MAX_INT, s2 = 1, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -(MAX_INT)-1);
	CHECK(ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "ADD underflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = ADD;
	s1 = -(MAX_INT)-1, s2 = -1, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == MAX_INT);
	CHECK(!ct->get_condition(OF));
	CHECK(ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "SUB within bounds", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = SUB;
	s1 = 200, s2 = 131, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 69);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "SUB overflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = SUB;
	s1 = MAX_INT, s2 = -1, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -(MAX_INT)-1);
	CHECK(ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "SUB underflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = SUB;
	s1 = -(MAX_INT)-1, s2 = 1, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == MAX_INT);
	CHECK(!ct->get_condition(OF));
	CHECK(ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "MUL within bounds", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = MUL;
	s1 = 200, s2 = 200, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 40000);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "MUL overflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = MUL;
	s1 = MAX_INT, s2 = MAX_INT, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 1);
	CHECK(ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "MUL underflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = MUL;
	s1 = MAX_INT, s2 = -MAX_INT, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -1);
	CHECK(!ct->get_condition(OF));
	CHECK(ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "QUOT within bounds", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = QUOT;
	s1 = 2043, s2 = 40, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 51);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "QUOT overflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = QUOT;
	s1 = -(MAX_INT)-1, s2 = -1, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -(MAX_INT)-1);
	CHECK(ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "QUOT halt", "[ex]")
{
	try {
		signed int s1, s2, s3;
		Mnemonic m;

		m = QUOT;
		s1 = 1, s2 = 0, s3 = 0;
		execute_instr(s1, s2, s3, m);
		FAIL(true);
	} catch (HaltException &e) {
	}
}

TEST_CASE_METHOD(EXFixture, "REM within bounds", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = REM;
	s1 = 2043, s2 = 40, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 3);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "REM halt", "[ex]")
{
	try {
		signed int s1, s2, s3;
		Mnemonic m;

		m = REM;
		s1 = 1, s2 = 0, s3 = 0;
		execute_instr(s1, s2, s3, m);
		FAIL(true);
	} catch (HaltException &e) {
	}
}

TEST_CASE_METHOD(EXFixture, "SFTR within bounds", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = SFTR;
	s1 = 1300, s2 = 6, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 20);
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
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = SFTL;
	s1 = 13, s2 = 6, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 832);
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
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = AND;
	s1 = 1234, s2 = 5678, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 1026);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "OR", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = OR;
	s1 = 1234, s2 = 5678, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 5886);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "NOT", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = NOT;
	s1 = 1234, s2 = -1, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -1235);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "XOR", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = XOR;
	s1 = 1234, s2 = 5678, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 4860);
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
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = CMP;
	s1 = -1, s2 = 0, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	// should not be changed
	CHECK(i->operands.integer.slot_one == -1);

	CHECK(!ct->get_condition(EQ));
	CHECK(!ct->get_condition(GT));
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "CMP equal", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = CMP;
	s1 = 20, s2 = 20, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	// should not be changed
	CHECK(i->operands.integer.slot_one == 20);

	CHECK(ct->get_condition(EQ));
	CHECK(!ct->get_condition(GT));
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "CMP greater", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = CMP;
	s1 = 21, s2 = 20, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	// should not be changed
	CHECK(i->operands.integer.slot_one == 21);

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

TEST_CASE_METHOD(EXFixture, "LOAD", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = LOAD;
	s1 = 42000, s2 = 0;
	s3 = -41958;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 42);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "SRDL", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "ADDI within bounds", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = ADDI;
	s1 = 42000, s2 = 0;
	s3 = -41958;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 42);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "ADDI overflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = ADDI;
	s1 = MAX_INT, s2 = 0, s3 = 1;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -(MAX_INT)-1);
	CHECK(ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "ADDI underflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = ADDI;
	s1 = -(MAX_INT)-1, s2 = 0, s3 = -1;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == MAX_INT);
	CHECK(!ct->get_condition(OF));
	CHECK(ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "SUBI within bounds", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = SUBI;
	s1 = 200, s2 = 0, s3 = 131;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 69);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "SUBI overflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = SUBI;
	s1 = -(MAX_INT)-1, s2 = 0, s3 = 1;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == MAX_INT);
	CHECK(!ct->get_condition(OF));
	CHECK(ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "SUBI underflow", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = SUBI;
	s1 = MAX_INT, s2 = 0, s3 = -1;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -(MAX_INT)-1);
	CHECK(ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "SFTRI within bounds", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = SFTRI;
	s1 = 1300, s2 = 0, s3 = 6;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 20);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "SFTRI overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "SFTRI underflow", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "SFTLI within bounds", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = SFTLI;
	s1 = 13, s2 = 0, s3 = 6;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 832);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "SFTLI overflow", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "SFTLI underflow", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "ANDI", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = ANDI;
	s1 = 1234, s2 = 0, s3 = 5678;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 1026);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "ORI", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = ORI;
	s1 = 1234, s2 = 0, s3 = 5678;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 5886);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "XORI", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = XORI;
	s1 = 1234, s2 = 0, s3 = 5678;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 4860);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "STORE", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = LOAD;
	s1 = 42000, s2 = 0, s3 = -41958;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 42);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "SRDS", "[ex]")
// {
// }

TEST_CASE_METHOD(EXFixture, "JMP", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = JMP;
	s1 = 42000, s2 = -41958, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 42);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "JRL", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = JRL;
	s1 = 100, s2 = 69, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 69);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "JAL", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = JAL;
	s1 = 42000, s2 = -41958, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 42);
	CHECK(!ct->get_condition(OF));
	CHECK(!ct->get_condition(UF));

	delete i;
}

TEST_CASE_METHOD(EXFixture, "BEQ no cond", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = BEQ;
	s1 = 100, s2 = 50, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -1);

	delete i;
}

TEST_CASE_METHOD(EXFixture, "BEQ", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = BEQ;
	s1 = 100, s2 = 50, s3 = 0;
	this->ct->set_condition(EQ, true);
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 50);

	delete i;
}

TEST_CASE_METHOD(EXFixture, "BGT no cond", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = BGT;
	s1 = 100, s2 = 50, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -1);

	delete i;
}

TEST_CASE_METHOD(EXFixture, "BGT", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = BGT;
	s1 = 100, s2 = 50, s3 = 0;
	this->ct->set_condition(GT, true);
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 50);

	delete i;
}

TEST_CASE_METHOD(EXFixture, "BUF no cond", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = BUF;
	s1 = 100, s2 = -42027, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -1);

	delete i;
}

TEST_CASE_METHOD(EXFixture, "BUF", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = BUF;
	s1 = 100, s2 = 50, s3 = 0;
	this->ct->set_condition(UF, true);
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 50);

	delete i;
}

TEST_CASE_METHOD(EXFixture, "BOF no cond", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = BOF;
	s1 = 100, s2 = -42027, s3 = 0;
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == -1);

	delete i;
}

TEST_CASE_METHOD(EXFixture, "BOF", "[ex]")
{
	signed int s1, s2, s3;
	Mnemonic m;
	InstrDTO *i;

	m = BOF;
	s1 = 100, s2 = 50, s3 = 0;
	this->ct->set_condition(OF, true);
	i = execute_instr(s1, s2, s3, m);

	CHECK(i->operands.integer.slot_one == 50);

	delete i;
}

// TEST_CASE_METHOD(EXFixture, "PUSH", "[ex]")
// {
// }

// TEST_CASE_METHOD(EXFixture, "POP", "[ex]")
// {
// }
