#include "ex.h"
#include "accessor.h"
#include "definitions.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"
#include <unordered_map>

// clang-format off
#define INIT_INSTRUCTION(mnemonic, body)					\
	{mnemonic, [this](signed int &s1, signed int s2, signed int s3) { \
		body; \
	}}
// clang-format on

EX::EX(Stage *stage) : Stage(stage)
{
	this->id = EXEC;
	instr_map = {

		/* R type instructions */
		INIT_INSTRUCTION(
			ADD,
			{
				s1 = s1 + s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SUB,
			{
				s1 = s1 - s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			MUL,
			{
				s1 = s1 * s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			QUOT,
			{
				s1 = s1 / s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			REM,
			{
				s1 = s1 % s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SFTR,
			{
				s1 = s1 >> s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SFTL,
			{
				s1 = s1 << s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			AND,
			{
				s1 = s1 & s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			OR,
			{
				s1 = s1 | s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			NOT,
			{
				s1 = ~s1;
				(void)s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			XOR,
			{
				s1 = s1 ^ s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			ADDV,
			{
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SUBV,
			{
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			MULV,
			{
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			DIVV,
			{
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			CMP,
			{
				(s1 > s2) ? this->set_condition(GT, true)
						  : this->set_condition(GT, false);
				(s1 == s2) ? this->set_condition(EQ, true)
						   : this->set_condition(EQ, false);
				(void)s3;
			}),

		INIT_INSTRUCTION(
			CEV,
			{
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		/* I type instructions */
		INIT_INSTRUCTION(
			LOAD,
			{
				s1 = s1 + s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			LOADV,
			{
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			ADDI,
			{
				s1 = s1 + s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SUBI,
			{
				s1 = s1 - s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SFTRI,
			{
				s1 = s1 >> s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SFTLI,
			{
				s1 = s1 << s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			ANDI,
			{
				s1 = s1 & s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			ORI,
			{
				s1 = s1 | s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			XORI,
			{
				s1 = s1 ^ s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			STORE,
			{
				s1 = s1 + s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			STOREV,
			{
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		/* J type instructions */
		INIT_INSTRUCTION(
			JMP,
			{
				s1 = s1 + s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			JRL,
			{
				s1 = this->pc + s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			JAL,
			{
				s1 = s1 + s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			BEQ,
			{
				s1 = this->pc + s2;
				(void)s3;
			}),

		INIT_INSTRUCTION(
			BGT,
			{
				s1 = this->pc + s2;
				(void)s3;
			}),

		INIT_INSTRUCTION(
			BUF,
			{
				s1 = this->pc + s2;
				(void)s3;
			}),

		INIT_INSTRUCTION(
			BOF,
			{
				s1 = this->pc + s2;
				(void)s3;
			}),

		INIT_INSTRUCTION(
			PUSH,
			{
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			POP,
			{
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		/* NOP */
		INIT_INSTRUCTION(
			NOP,
			{
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),
	};
}

void EX::advance_helper()
{
	signed int s1, s2, s3;
	Mnemonic m;

	m = this->curr_instr->get_mnemonic();
	s1 = this->curr_instr->get_s1();
	s2 = this->curr_instr->get_s2();
	s3 = this->curr_instr->get_s3();

	this->instr_map[m](s1, s2, s3);

	this->curr_instr->set_s1(s1);
	this->status = OK;
}
