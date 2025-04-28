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

Response ID::read_guard(signed int &v)
{
	Response r;
	if (this->is_checked_out(v))
		r = STALLED;
	else {
		r = OK;
		v = this->dereference_register<signed int>(v);
	}
	return r;
}

Response
ID::read_vec_guard(signed int v, std::array<signed int, V_R_LIMIT> &vrs)
{
	Response r;
	if (this->is_checked_out(v))
		r = STALLED;
	else {
		r = OK;
		vrs = this->dereference_register<std::array<signed int, V_R_LIMIT>>(v);
	}
	return r;
}

void ID::write_guard(signed int &v)
{
	// zero register shouldn't be written.
	if (v != 0) {
		// keep track in the instrDTO for displaying to user and writeback
		// keep track in checked_out so we can still access this information!
		this->checked_out.push_back(v);
		this->curr_instr->checked_out = v;
	}
	v = this->dereference_register<signed int>(v);
}

void ID::write_vec_guard(signed int v, std::array<signed int, V_R_LIMIT> &vrs)
{

	// zero register shouldn't be written.
	if (v != 0) {
		// keep track in the instrDTO for displaying to user and writeback
		// keep track in checked_out so we can still access this information!
		this->checked_out.push_back(v);
		this->curr_instr->checked_out = v;
	}
	vrs = this->dereference_register<std::array<signed int, V_R_LIMIT>>(v);
}

void ID::advance_helper()
{
	signed int s1;

	if (this->curr_instr->mnemonic == NOP)
		this->status = OK;
	else {
		// instuction in bits in s1
		s1 = this->curr_instr->slot_A;
		get_instr_fields(s1);
	}
}

void ID::get_instr_fields(signed int &s1)
{
	unsigned int type;
	Mnemonic m;
	this->split_instr(s1, type, m);
	this->curr_instr->mnemonic = m;
	switch (type) {
	case 0b00:
		this->decode_R_type(s1);
		break;
	case 0b01:
		this->decode_I_type(s1);
		break;
	case 0b10:
		this->decode_J_type(s1);
		break;
	case 0b11:
		// not defined, m = NOP
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
	r = this->read_guard(vlen_reg);
	vlen_reg = vlen_reg & 0xf;
	if (r == OK) {
		if (vlen_reg > V_R_LIMIT) {
			this->curr_instr->slot_A = V_R_LIMIT;
		} else {
			this->curr_instr->slot_A = vlen_reg;
		}
	}
	return r;
}

void ID::decode_R_type(signed int &s1)
{
	unsigned int s0b, s1b, s2b;
	signed int s2, s3;
	Response r1, r2;
	Response r3 = OK;

	s0b = REG_SIZE;
	s1b = s0b + REG_SIZE;
	s2b = s1b + REG_SIZE;
	s3 = GET_MID_BITS(s1, s1b, s2b);
	s2 = GET_MID_BITS(s1, s0b, s1b);
	s1 = GET_LS_BITS(s1, s0b);

	if (this->is_vector_type(this->curr_instr->mnemonic)) {
		r1 = this->read_vec_guard(
			s1, this->curr_instr->operands.vector.slot_one);
		r2 = this->read_vec_guard(
			s2, this->curr_instr->operands.vector.slot_two);
		r3 = this->set_vlen();
	} else {
		r1 = this->read_guard(s1);
		this->curr_instr->operands.integer.slot_one = s1;
		r2 = this->read_guard(s2);
		this->curr_instr->operands.integer.slot_two = s2;
	}

	this->status = (r1 == OK && r2 == OK && r3 == OK) ? OK : STALLED;

	switch (this->curr_instr->mnemonic) {
	case CMP:
	case CEV:
		break;
	case ADDV:
	case SUBV:
	case MULV:
	case DIVV:
		if (this->status == OK) {
			this->write_vec_guard(
				s3, this->curr_instr->operands.vector.slot_three);
		}
		break;
	default:
		if (this->status == OK) {
			this->write_guard(s3);
			this->curr_instr->operands.integer.slot_three = s3;
		}
	}
}

void ID::decode_I_type(signed int &s1)
{
	unsigned int s0b, s1b, s2b;
	signed int s2, s3;
	Response r1, r2;
	Response r3 = OK;

	s0b = REG_SIZE;
	s1b = s0b + REG_SIZE;
	s2b = WORD_SPEC - LINE_SPEC - OPCODE_SIZE;
	// s3 is immediate
	s3 = GET_BITS_SIGN_EXTEND(s1, s1b, s2b);

	switch (this->curr_instr->mnemonic) {
	case STORE:
		this->curr_instr->operands.integer.slot_three = s3;
		s2 = GET_MID_BITS(s1, s0b, s1b);
		s1 = GET_LS_BITS(s1, s0b);

		// both operands are read values
		// s1 is base address
		r1 = this->read_guard(s1);
		this->curr_instr->operands.integer.slot_one = s1;
		// s2 is value to be stored
		r2 = this->read_guard(s2);
		this->curr_instr->operands.integer.slot_two = s2;
		this->status = (r1 == OK && r2 == OK) ? OK : STALLED;
		return;
	case STOREV:
		this->curr_instr->operands.load_store_vector.immediate = s3;
		s2 = GET_MID_BITS(s1, s0b, s1b);
		s1 = GET_LS_BITS(s1, s0b);

		// base address
		r1 = this->read_guard(s1);
		this->curr_instr->operands.load_store_vector.base_addr = s1;
		// vector value to be stored
		r2 = this->read_vec_guard(
			s2, this->curr_instr->operands.load_store_vector.vector_register);
		r3 = this->set_vlen();

		this->status = (r1 == OK && r2 == OK && r3 == OK) ? OK : STALLED;
		return;
	case LOADV:
		this->curr_instr->operands.load_store_vector.immediate = s3;
		s2 = GET_LS_BITS(s1, s0b);
		s1 = GET_MID_BITS(s1, s0b, s1b);
		// base address
		r1 = this->read_guard(s1);
		this->curr_instr->operands.load_store_vector.base_addr = s1;
		r3 = this->set_vlen();
		if (r1 == OK && r3 == OK)
			// vector destination
			this->write_vec_guard(
				s2,
				this->curr_instr->operands.load_store_vector.vector_register);
		this->status = (r1 == OK && r3 == OK) ? OK : STALLED;
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

	r1 = this->read_guard(s1);
	this->curr_instr->operands.integer.slot_one = s1;
	if (r1 == OK) {
		this->write_guard(s2);
		this->curr_instr->operands.integer.slot_two = s2;
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
		r1 = this->read_guard(s2);
		this->curr_instr->operands.integer.slot_one = s1;
		this->curr_instr->operands.integer.slot_two = s2;
		r2 = (this->is_checked_out(s3)) ? STALLED
										: OK; // we read the stack pointer
		if (r1 == OK && r2 == OK) {
			this->write_guard(s3); // we write the stack pointer
			this->curr_instr->operands.integer.slot_three = s3;
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
			this->write_guard(s2);
			this->curr_instr->operands.integer.slot_two = s2;
			this->write_guard(s3); // we write the stack pointer
			this->curr_instr->operands.integer.slot_three = s3;
		}
		this->status = r1;
		break;
	case RET:
		s1 = 1; // link register
		[[fallthrough]];
	default:
		this->status = this->read_guard(s1);
		if(this->status == OK){
			this->curr_instr->operands.integer.slot_one = s1;
			this->curr_instr->operands.integer.slot_two = s2;
			this->curr_instr->operands.integer.slot_three = s3;
		}
	}
}
