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

#include "wb.h"
#include "instr.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"
#include <algorithm>
#include <array>

void WB::advance_helper()
{
	if (this->curr_instr->mnemonic != NOP) {
		if (this->curr_instr->checked_out > 0)
			this->write_handler();
		else if (this->should_jump())
			this->jump_handler();
	}
	this->status = OK;
}

void WB::write_handler()
{
	signed int reg;

	if (this->checked_out.size() < 1)
		throw std::runtime_error("instruction tried to pop a register out of "
								 "an empty queue during writeback.");

	if (this->curr_instr->mnemonic == POP) {
		// POP performs a second register write
		reg = this->checked_out.front();
		this->checked_out.pop_front();
		this->store_register<signed int>(
			reg, this->curr_instr->operands.integer.slot_three);
	}

	this->checked_out.pop_front();
	reg = this->curr_instr->checked_out;

	switch (this->curr_instr->type) {
	case SI_INT:
		this->store_register<signed int>(
			reg, this->curr_instr->operands.integer.slot_one);
		break;
	case S_VECT:
	case R_VECT:
		this->store_register<std::array<signed int, V_R_LIMIT>>(
			reg, this->copy_extra_vector_elements());
		break;
	}
}

void WB::jump_handler()
{
	if (this->curr_instr->operands.integer.slot_one > 0) {
		if (this->curr_instr->mnemonic == JAL)
			this->gprs[1] = this->curr_instr->slot_B + 1;
		this->pc = this->curr_instr->operands.integer.slot_one;
		this->checked_out = {};
		this->next->squash();
	}
}

// TODO clean this up...
bool WB::should_jump()
{
	vector<Mnemonic> Js = {JMP, JRL, JAL, BEQ, BGT, BUF, BOF, PUSH, POP, RET};

	if (find(Js.begin(), Js.end(), this->curr_instr->mnemonic) != Js.end())
		return true;
	else
		return false;
}

std::array<signed int, V_R_LIMIT> WB::copy_extra_vector_elements()
{
	int i;
	std::array<signed int, V_R_LIMIT> v;

	if (this->curr_instr->type == R_VECT) {
		if (this->curr_instr->slot_B == 0) {
			v = {0};
			return v;
		}
	}

	v = this->curr_instr->operands.vector.slot_one;
	for (i = V_R_LIMIT - 1; i >= this->curr_instr->slot_B; --i) {
		v[i] = this->curr_instr->operands.vector.slot_three[i];
	}
	return v;
}
