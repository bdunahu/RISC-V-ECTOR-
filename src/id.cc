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

#include "id.h"
#include "instr.h"
#include "instrDTO.h"
#include "logger.h"
#include "response.h"
#include "stage.h"

void ID::advance_helper()
{
	if (this->curr_instr->mnemonic == NOP)
		this->status = OK;
	else {
		// instuction in bits in s1
		get_instr_fields(this->curr_instr->slot_A);
	}
}

void ID::get_instr_fields(signed int instr_bits)
{
	unsigned int type;
	Mnemonic m;
	this->split_instr(instr_bits, type, m);
	this->curr_instr->mnemonic = m;
	this->curr_instr->type = instr::get_field_types(m);
	switch (type) {
	case 0b00:
		this->decode_R_type(instr_bits);
		break;
	case 0b01:
		this->decode_I_type(instr_bits);
		break;
	case 0b10:
		this->decode_J_type(instr_bits);
		break;
	case 0b11:
		this->status = OK;
	}
}

void ID::split_instr(signed int &raw, unsigned int &type, Mnemonic &m)
{
	unsigned int opcode, opcode_size;

	type = GET_LS_BITS(raw, TYPE_SIZE);
	opcode_size = (type == 0b0) ? R_OPCODE_SIZE : OPCODE_SIZE;
	opcode = GET_MID_BITS(raw, TYPE_SIZE, TYPE_SIZE + opcode_size);
	try {
		m = instr::mnemonic_map.at((opcode << TYPE_SIZE) + type);
	} catch (std::out_of_range const &) {
		m = NOP;
	}

	raw = (unsigned int)raw >> (TYPE_SIZE + opcode_size);
}

Response ID::set_vlen()
{
	signed int vlen_reg = 4;
	Response r;
	r = this->read_guard<signed int>(vlen_reg, vlen_reg);
	vlen_reg = vlen_reg & 0xf;
	if (r == OK) {
		if (vlen_reg > V_R_LIMIT) {
			this->curr_instr->slot_B = V_R_LIMIT;
		} else {
			this->curr_instr->slot_B = vlen_reg;
		}
	}
	return r;
}

void ID::decode_R_type(signed int &s1)
{
	unsigned int s0b, s1b, s2b;
	signed int s2, s3;
	Response r1, r2, r3;

	s0b = REG_SIZE;
	s1b = s0b + REG_SIZE;
	s2b = s1b + REG_SIZE;
	s3 = GET_MID_BITS(s1, s1b, s2b);
	s2 = GET_MID_BITS(s1, s0b, s1b);
	s1 = GET_LS_BITS(s1, s0b);

	switch (this->curr_instr->type) {
	case SI_INT:
		r1 = this->read_guard<signed int>(
			s1, this->curr_instr->operands.integer.slot_one);
		r2 = this->read_guard<signed int>(
			s2, this->curr_instr->operands.integer.slot_two);
		r3 = OK;
		break;
	case R_VECT:
		r1 = this->read_guard<std::array<signed int, V_R_LIMIT>>(
			s1, this->curr_instr->operands.vector.slot_one);
		r2 = this->read_guard<std::array<signed int, V_R_LIMIT>>(
			s2, this->curr_instr->operands.vector.slot_two);
		r3 = this->set_vlen();
		break;
	case S_VECT:
		// store the second field in s1, to keep execute+mem consistent
		r1 = this->read_guard<std::array<signed int, V_R_LIMIT>>(
			s2, this->curr_instr->operands.s_vector.slot_one);
		r2 = this->read_guard<signed int>(
			s1, this->curr_instr->operands.s_vector.slot_two);
		r3 = this->set_vlen();
		break;
	}

	this->status = (r1 == OK && r2 == OK && r3 == OK) ? OK : STALLED;

	if (this->status == OK) {
		switch (this->curr_instr->mnemonic) {
		case CMP:
		case CEV:
			break;
		case ADDV:
		case SUBV:
		case MULV:
		case DIVV:
			this->curr_instr->operands.vector.slot_three =
				this->write_guard<std::array<signed int, V_R_LIMIT>>(s3);
			break;
		case ROTV:
		case SRDL:
			this->curr_instr->operands.s_vector.slot_three =
				this->write_guard<std::array<signed int, V_R_LIMIT>>(s3);
			break;
		case SRDS:
			r1 = this->read_guard<std::array<signed int, V_R_LIMIT>>(
				s3, this->curr_instr->operands.s_vector.slot_three);
			if (r1 != OK) {
				this->status = STALLED;
			}
			break;
		default:
			this->curr_instr->operands.integer.slot_three =
				this->write_guard<signed int>(s3);
		}
	}
}

void ID::decode_I_type(signed int &s1)
{
	unsigned int s0b, s1b, s2b;
	signed int s2, s3;
	Response r1, r2;

	s0b = REG_SIZE;
	s1b = s0b + REG_SIZE;
	s2b = WORD_SPEC - LINE_SPEC - OPCODE_SIZE;
	// s3 is slot_two
	s3 = GET_BITS_SIGN_EXTEND(s1, s1b, s2b);

	switch (this->curr_instr->mnemonic) {
	case STORE:
		this->curr_instr->operands.integer.slot_three = s3;
		s2 = GET_MID_BITS(s1, s0b, s1b);
		s1 = GET_LS_BITS(s1, s0b);

		// both operands are read values
		// s1 is base address
		r1 = this->read_guard<signed int>(s1, s1);
		this->curr_instr->operands.integer.slot_one = s1;
		// s2 is value to be stored
		r2 = this->read_guard<signed int>(s2, s2);
		this->curr_instr->operands.integer.slot_two = s2;
		this->status = (r1 == OK && r2 == OK) ? OK : STALLED;
		return;
	case LOAD:
		this->curr_instr->operands.integer.slot_three = s3;
		s2 = GET_LS_BITS(s1, s0b);
		s1 = GET_MID_BITS(s1, s0b, s1b);
		break;
	default:
		this->curr_instr->operands.integer.slot_three = s3;
		s2 = GET_MID_BITS(s1, s0b, s1b);
		s1 = GET_LS_BITS(s1, s0b);
	}

	r1 = this->read_guard<signed int>(s1, s1);
	this->curr_instr->operands.integer.slot_one = s1;
	if (r1 == OK) {
		this->curr_instr->operands.integer.slot_two =
			this->write_guard<int>(s2);
	}
	this->status = r1;
}

void ID::decode_J_type(signed int &s1)
{
	Response r1, r2;
	signed int s2, s3;
	unsigned int s0b, s1b;

	s0b = REG_SIZE;
	s1b = WORD_SPEC - LINE_SPEC - OPCODE_SIZE;
	s3 = 0;
	s2 = GET_BITS_SIGN_EXTEND(s1, s0b, s1b);
	s1 = GET_LS_BITS(s1, s0b);

	switch (this->curr_instr->mnemonic) {
	case PUSH:
		s2 = s1; // source
		s3 = 2;	 // stack pointer
		s1 = -1; // increment amount

		this->curr_instr->operands.integer.slot_one = s1;
		r1 = this->read_guard<signed int>(
			s2, this->curr_instr->operands.integer.slot_two);
		r2 = (this->is_checked_out(s3)) ? STALLED
										: OK; // we read the stack pointer
		if (r1 == OK && r2 == OK) {
			// we write the stack pointer
			this->curr_instr->operands.integer.slot_three =
				this->write_guard<int>(s3);
		}
		this->status = (r1 == OK && r2 == OK) ? OK : STALLED;
		break;
	case POP:
		s2 = s1; // destination
		s3 = 2;	 // stack pointer
		s1 = 1;	 // increment amount
		this->curr_instr->operands.integer.slot_one = s1;
		r1 = (this->is_checked_out(s3)) ? STALLED
										: OK; // we read the stack pointer
		if (r1 == OK) {
			this->curr_instr->operands.integer.slot_two =
				this->write_guard<int>(s2);
			// we write the stack pointer
			this->curr_instr->operands.integer.slot_three =
				this->write_guard<int>(s3);
		}
		this->status = r1;
		break;
	case RET:
		s1 = 1; // link register
		[[fallthrough]];
	default:
		this->status = this->read_guard<signed int>(
			s1, this->curr_instr->operands.integer.slot_one);
		if (this->status == OK) {
			this->curr_instr->operands.integer.slot_two = s2;
			this->curr_instr->operands.integer.slot_three = s3;
		}
	}
}
