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
#include "instrDTO.h"
#include "pipe_spec.h"
#include "response.h"
#include "stage.h"
#include <unordered_map>

// Switch statements for each instruction
void EX::advance_helper()
{
	signed int s1, s2, s3;
	std::array<signed int, V_R_LIMIT> v1, v2, v3;
	signed int v_len, v_immediate, v_base_addr;
	unsigned int pc;
	Mnemonic m;

	s1 = 0, s2 = 0, s3 = 0;
	v1 = {0}, v2 = {0}, v3 = {0};
	v_len = 0, v_immediate = 0, v_base_addr = 0;
	m = this->curr_instr->mnemonic;
	pc = this->curr_instr->slot_B;

	if (this->is_vector_type(m)) {
		if (this->curr_instr->mnemonic != LOADV &&
			this->curr_instr->mnemonic != STOREV) {
			v1 = this->curr_instr->operands.vector.slot_one;
			v2 = this->curr_instr->operands.vector.slot_two;
			v3 = this->curr_instr->operands.vector.slot_three;
		} else {
			v_immediate =
				this->curr_instr->operands.load_store_vector.immediate;
			v_base_addr =
				this->curr_instr->operands.load_store_vector.base_addr;
		}
		v_len = this->curr_instr->slot_A;
		if (v_len == 0) {
			// clear destination vector reg
			v1.fill(0);
		}
	} else {
		s1 = this->curr_instr->operands.integer.slot_one;
		s2 = this->curr_instr->operands.integer.slot_two;
		s3 = this->curr_instr->operands.integer.slot_three;
	}

	if (this->is_logical(m)) {
		this->set_condition(OF, false);
		this->set_condition(UF, false);
	}

	switch (m) {
	case ADD:
		this->set_condition(OF, ADDITION_OF_GUARD(s1, s2));
		this->set_condition(UF, ADDITION_UF_GUARD(s1, s2));
		s1 = s1 + s2;
		break;

	case SUB:
		this->set_condition(OF, SUBTRACTION_OF_GUARD(s1, s2));
		this->set_condition(UF, SUBTRACTION_UF_GUARD(s1, s2));
		s1 = s1 - s2;
		break;

	case MUL:
		this->set_condition(OF, MULTIPLICATION_OF_GUARD(s1, s2));
		this->set_condition(UF, MULTIPLICATION_UF_GUARD(s1, s2));
		s1 = s1 * s2;
		break;

	case QUOT:
		this->handle_divide(s1, s2, false);
		break;

	case REM:
		this->handle_divide(s1, s2, true);
		break;

	case SFTR:
		s1 = s1 >> s2;
		break;

	case SFTL:
		s1 = s1 << s2;
		break;

	case AND:
		s1 = s1 & s2;
		break;

	case OR:
		s1 = s1 | s2;
		break;

	case XOR:
		s1 = s1 ^ s2;
		break;

	case NOT:
		s1 = ~s1;
		break;

	case CMP:
		(s1 > s2) ? this->set_condition(GT, true)
				  : this->set_condition(GT, false);
		(s1 == s2) ? this->set_condition(EQ, true)
				   : this->set_condition(EQ, false);
		break;

	case ADDI:
		this->set_condition(OF, ADDITION_OF_GUARD(s1, s3));
		this->set_condition(UF, ADDITION_UF_GUARD(s1, s3));
		s1 = s1 + s3;
		break;

	case SUBI:
		this->set_condition(OF, SUBTRACTION_OF_GUARD(s1, s3));
		this->set_condition(UF, SUBTRACTION_UF_GUARD(s1, s3));
		s1 = s1 - s3;
		break;

	case SFTRI:
		s1 = s1 >> s3;
		break;

	case SFTLI:
		s1 = s1 << s3;
		break;

	case ANDI:
		s1 = s1 & s3;
		break;

	case ORI:
		s1 = s1 | s3;
		break;

	case XORI:
		s1 = s1 ^ s3;
		break;

	case LOAD:
	case STORE:
	case PUSH:
	case POP:
		s1 = s1 + s3;
		break;

	case JMP:
	case JAL:
		s1 = s1 + s2;
		break;

	case JRL:
		s1 = pc + s2;
		break;

	case BEQ:
		(this->get_condition(EQ)) ? s1 = pc + s2 : s1 = -1;
		break;

	case BGT:
		(this->get_condition(GT)) ? s1 = pc + s2 : s1 = -1;
		break;

	case BUF:
		(this->get_condition(UF)) ? s1 = pc + s2 : s1 = -1;
		break;

	case BOF:
		(this->get_condition(OF)) ? s1 = pc + s2 : s1 = -1;
		break;

	case ADDV:
		for (int i = 0; i < v_len; i++) {
			this->set_condition(OF, ADDITION_OF_GUARD(v1[i], v2[i]));
			this->set_condition(UF, ADDITION_UF_GUARD(v1[i], v2[i]));
			v1[i] = v1[i] + v2[i];
		}
		break;
	case SUBV:
		for (int i = 0; i < v_len; i++) {
			this->set_condition(OF, SUBTRACTION_OF_GUARD(v1[i], v2[i]));
			this->set_condition(UF, SUBTRACTION_UF_GUARD(v1[i], v2[i]));
			v1[i] = v1[i] - v2[i];
		}
		break;
	case MULV:
		for (int i = 0; i < v_len; i++) {
			this->set_condition(OF, MULTIPLICATION_OF_GUARD(v1[i], v2[i]));
			this->set_condition(UF, MULTIPLICATION_UF_GUARD(v1[i], v2[i]));
			v1[i] = v1[i] * v2[i];
		}
		break;
	case DIVV:
		for (int i = 0; i < v_len; i++) {
			this->handle_divide(v1[i], v2[i], false);
		}
		break;
	case CEV:
		int i;
		for (i = 0; i < v_len; i++) {
			if (v1[i] != v2[i]) {
				break;
			}
		}
		if (i == v_len) {
			this->set_condition(EQ, true);
		} else {
			this->set_condition(EQ, false);
		}
		break;
	case LOADV:
	case STOREV:
		v_base_addr = v_base_addr + v_immediate;
		break;

	case RET:
	case NOP:
		break;
	}
	if (this->is_vector_type(m)) {
		if (this->curr_instr->mnemonic != LOADV &&
			this->curr_instr->mnemonic != STOREV) {
			this->curr_instr->operands.vector.slot_one = v1;
		} else {
			this->curr_instr->operands.load_store_vector.base_addr =
				v_base_addr;
		}
	} else {
		this->curr_instr->operands.integer.slot_one = s1;
	}
	this->status = OK;
}

void EX::handle_divide(signed int &s1, signed int s2, bool is_mod)
{
	this->set_condition(OF, DIVISION_OF_GUARD(s1, s2));
	this->set_condition(UF, false);
	if (s2 == 0) {
		// handle everything here
		this->curr_instr->operands.integer.slot_one = MAX_INT;
		this->status = OK;
		throw HaltException();
	} else if ((s1 == -(MAX_INT)-1) && s2 == -1) {
		// undefined in C++
		s1 = -(MAX_INT)-1;
	} else {
		s1 = (is_mod) ? (s1 % s2) : (s1 / s2);
	}
}
