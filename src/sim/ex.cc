#include "ex.h"
#include "accessor.h"
#include "definitions.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"
#include <unordered_map>

// clang-format off
#define INIT_INSTRUCTION(mnemonic, body)					\
	{mnemonic, [this](signed int &s1, signed int s2) { \
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
				(void)this;
			}),

		INIT_INSTRUCTION(
			SUB,
			{
				s1 = s1 - s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			MUL,
			{
				s1 = s1 * s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			QUOT,
			{
				s1 = s1 / s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			REM,
			{
				s1 = s1 % s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SFTR,
			{
				s1 = s1 >> s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SFTL,
			{
				s1 = s1 << s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			AND,
			{
				s1 = s1 & s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			OR,
			{
				s1 = s1 | s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			NOT,
			{
				s1 = ~s1;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			XOR,
			{
				s1 = s1 ^ s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			ADDV,
			{
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SUBV,
			{
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			MULV,
			{
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			DIVV,
			{
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
			}),

		INIT_INSTRUCTION(
			CEV,
			{
				(void)s2;
				(void)s1;
				(void)this;
			}),

		/* I type instructions */
		INIT_INSTRUCTION(LOAD, {}),

		INIT_INSTRUCTION(LOADV, {}),

		INIT_INSTRUCTION(ADDI, {}),

		INIT_INSTRUCTION(SUBI, {}),

		INIT_INSTRUCTION(SFTRI, {}),

		INIT_INSTRUCTION(SFTL, {}),

		INIT_INSTRUCTION(ANDI, {}),

		INIT_INSTRUCTION(ORI, {}),

		INIT_INSTRUCTION(XORI, {}),

		INIT_INSTRUCTION(STORE, {}),

		INIT_INSTRUCTION(STOREV, {}),

		/* J type instructions */
		INIT_INSTRUCTION(JMP, {}),

		INIT_INSTRUCTION(JRL, {}),

		INIT_INSTRUCTION(JAL, {}),

		INIT_INSTRUCTION(BEQ, {}),

		INIT_INSTRUCTION(BGT, {}),

		INIT_INSTRUCTION(BUF, {}),

		INIT_INSTRUCTION(BOF, {}),

		INIT_INSTRUCTION(PUSH, {}),

		INIT_INSTRUCTION(POP, {}),

		/* NOP */
		INIT_INSTRUCTION(
			NOP,
			{
				(void)s2;
				(void)s1;
				(void)this;
			}),
	};
}

void EX::advance_helper() {
	signed int s1, s2;
	Mnemonic m;
	
	// it may be good to ensure we are not doing
	// work that has already been done
	if (this->curr_instr) {
		m = this->curr_instr->get_mnemonic();
		s1 = this->curr_instr->get_s1();
		s2 = this->curr_instr->get_s2();

		this->instr_map[m](s1, s2);

		this->curr_instr->set_s1(s1);
		this->status = OK;
	}
}
