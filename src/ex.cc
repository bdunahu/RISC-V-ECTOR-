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
#include "instr.h"
#include "instrDTO.h"
#include "pipe_spec.h"
#include "response.h"
#include "stage.h"
#include <stdexcept>
#include <unordered_map>

void EX::handle_int_operations(
	signed int &s1, signed int s2, signed int s3, Mnemonic m, unsigned int pc)
{
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
		this->set_condition(OF, false);
		this->set_condition(UF, false);
		s1 = s1 & s2;
		break;

	case OR:
		this->set_condition(OF, false);
		this->set_condition(UF, false);
		s1 = s1 | s2;
		break;

	case XOR:
		this->set_condition(OF, false);
		this->set_condition(UF, false);
		s1 = s1 ^ s2;
		break;

	case NOT:
		this->set_condition(OF, false);
		this->set_condition(UF, false);
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
		this->set_condition(OF, false);
		this->set_condition(UF, false);
		s1 = s1 & s3;
		break;

	case ORI:
		this->set_condition(OF, false);
		this->set_condition(UF, false);
		s1 = s1 | s3;
		break;

	case XORI:
		this->set_condition(OF, false);
		this->set_condition(UF, false);
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

	case RET:
	case NOP:
		break;

	default:
		throw std::invalid_argument(
			"handle_int_operations received a vector operation!");
	}
}

void EX::handle_vector_operations(
	std::array<signed int, V_R_LIMIT> &s1,
	std::array<signed int, V_R_LIMIT> s2,
	Mnemonic m,
	unsigned int v_len)
{
	unsigned int i;

	switch (m) {
	case ADDV:
		for (i = 0; i < v_len; i++) {
			this->set_condition(OF, ADDITION_OF_GUARD(s1[i], s2[i]));
			this->set_condition(UF, ADDITION_UF_GUARD(s1[i], s2[i]));
			s1[i] = s1[i] + s2[i];
		}
		break;
	case SUBV:
		for (i = 0; i < v_len; i++) {
			this->set_condition(OF, SUBTRACTION_OF_GUARD(s1[i], s2[i]));
			this->set_condition(UF, SUBTRACTION_UF_GUARD(s1[i], s2[i]));
			s1[i] = s1[i] - s2[i];
		}
		break;
	case MULV:
		for (i = 0; i < v_len; i++) {
			this->set_condition(OF, MULTIPLICATION_OF_GUARD(s1[i], s2[i]));
			this->set_condition(UF, MULTIPLICATION_UF_GUARD(s1[i], s2[i]));
			s1[i] = s1[i] * s2[i];
		}
		break;
	case DIVV:
		for (i = 0; i < v_len; i++) {
			this->handle_divide(s1[i], s2[i], false);
		}
		break;
	case CEV:

		for (i = 0; i < v_len; i++) {
			if (s1[i] != s2[i]) {
				break;
			}
		}
		if (i == v_len) {
			this->set_condition(EQ, true);
		} else {
			this->set_condition(EQ, false);
		}
		break;

	default:
		throw std::invalid_argument(
			"handle_vector_operations received an integer operation!");
	}
}

void EX::handle_i_vector_operations(signed int &s1, signed int s2, Mnemonic m)
{
	switch (m) {
	case LOADV:
	case STOREV:
		s1 = s1 + s2;
		break;

	case RET:
	case NOP:
		break;

	default:
		throw std::invalid_argument("handle_i_vector_operations did not "
									"receive a LOADV or STOREV operation!");
	}
}

void EX::advance_helper()
{
	unsigned int v_len_or_pc;
	Mnemonic m;
	m = this->curr_instr->mnemonic;
	v_len_or_pc = this->curr_instr->slot_B;

	if (this->curr_instr->type == FieldType::SI_INT) {
		handle_int_operations(
			this->curr_instr->operands.integer.slot_one,
			this->curr_instr->operands.integer.slot_two,
			this->curr_instr->operands.integer.slot_three, m, v_len_or_pc);
	} else if (this->curr_instr->type == FieldType::R_VECT) {
		handle_vector_operations(
			this->curr_instr->operands.vector.slot_one,
			this->curr_instr->operands.vector.slot_two, m, v_len_or_pc);
	} else {
		handle_i_vector_operations(
			this->curr_instr->operands.i_vector.slot_one,
			this->curr_instr->operands.i_vector.slot_two, m);
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
