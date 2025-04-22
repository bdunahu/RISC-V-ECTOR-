// Simulator for the RISC-V[ECTOR] mini-ISA
// Copyright (C) 2025 Siddarth Suresh
// Copyright (C) 2025 bdunahu

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "ex.h"
#include "accessor.h"
#include "instrDTO.h"
#include "pipe_spec.h"
#include "response.h"
#include "stage.h"
#include <unordered_map>

// clang-format off
#define INIT_INSTRUCTION(mnemonic, body)					\
	{mnemonic, [this](signed int &s1, signed int s2, signed int s3, unsigned int pc) {	\
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
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SUB,
			{
				s1 = s1 - s2;
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			MUL,
			{
				s1 = s1 * s2;
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			QUOT,
			{
				this->handle_divide(s1, s2, false);
				(void)pc;
				(void)s3;
			}),

		INIT_INSTRUCTION(
			REM,
			{
				this->handle_divide(s1, s2, true);
				s1 = s1 % s2;
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SFTR,
			{
				s1 = s1 >> s2;
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SFTL,
			{
				s1 = s1 << s2;
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			AND,
			{
				s1 = s1 & s2;
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			OR,
			{
				s1 = s1 | s2;
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			NOT,
			{
				s1 = ~s1;
				(void)pc;
				(void)s3;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			XOR,
			{
				s1 = s1 ^ s2;
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			ADDV,
			{
				(void)pc;
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SUBV,
			{
				(void)pc;
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			MULV,
			{
				(void)pc;
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			DIVV,
			{
				(void)pc;
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
				(void)pc;
				(void)s3;
			}),

		INIT_INSTRUCTION(
			CEV,
			{
				(void)pc;
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
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			LOADV,
			{
				(void)pc;
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		INIT_INSTRUCTION(
			ADDI,
			{
				s1 = s1 + s3;
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SUBI,
			{
				s1 = s1 - s3;
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SFTRI,
			{
				s1 = s1 >> s3;
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			SFTLI,
			{
				s1 = s1 << s3;
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			ANDI,
			{
				s1 = s1 & s3;
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			ORI,
			{
				s1 = s1 | s3;
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			XORI,
			{
				s1 = s1 ^ s3;
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			STORE,
			{
				s1 = s1 + s3;
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			STOREV,
			{
				(void)pc;
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
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			JRL,
			{
				s1 = pc + s2;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			JAL,
			{
				s1 = s1 + s2;
				(void)pc;
				(void)s3;
				(void)this;
			}),

		INIT_INSTRUCTION(
			BEQ,
			{
				(this->get_condition(EQ)) ? s1 = pc + s2 : s1 = -1;
				(void)s3;
			}),

		INIT_INSTRUCTION(
			BGT,
			{
				(this->get_condition(GT)) ? s1 = pc + s2 : s1 = -1;
				(void)s3;
			}),

		INIT_INSTRUCTION(
			BUF,
			{
				(this->get_condition(UF)) ? s1 = pc + s2 : s1 = -1;
				(void)s3;
			}),

		INIT_INSTRUCTION(
			BOF,
			{
				(this->get_condition(OF)) ? s1 = pc + s2 : s1 = -1;
				(void)s3;
			}),

		INIT_INSTRUCTION(
			PUSH,
			{
				s1 = s1 + s3;
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			POP,
			{
				s1 = s1 + s3;
				(void)pc;
				(void)s2;
				(void)this;
			}),

		INIT_INSTRUCTION(
			RET,
			{
				(void)pc;
				(void)s3;
				(void)s2;
				(void)s1;
				(void)this;
			}),

		/* NOP */
		INIT_INSTRUCTION(
			NOP,
			{
				(void)pc;
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
	unsigned int pc;
	Mnemonic m;

	m = this->curr_instr->get_mnemonic();
	s1 = this->curr_instr->get_s1();
	s2 = this->curr_instr->get_s2();
	s3 = this->curr_instr->get_s3();
	pc = this->curr_instr->get_pc();

	this->instr_map[m](s1, s2, s3, pc);

	this->curr_instr->set_s1(s1);
	this->status = OK;
}

void EX::handle_divide(signed int &s1, signed int s2, bool is_mod)
{
	if (s2 == 0) {
		// handle everything here
		this->curr_instr->set_s1(MAX_INT);
		this->status = OK;
		throw HaltException();
	} else {
		s1 = (is_mod) ? s1 % s2 : s1 / s2;
	}
}
