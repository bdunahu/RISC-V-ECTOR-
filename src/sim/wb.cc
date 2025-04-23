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
#include "instrDTO.h"
#include "response.h"
#include "stage.h"
#include <array>
#include <algorithm>

void WB::advance_helper()
{
	if (this->curr_instr->get_mnemonic() != NOP &&
		this->curr_instr->get_type() != INV) {
		if (this->curr_instr->get_checked_out() > 0)
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

	if (this->curr_instr->get_mnemonic() == POP) {
		// POP performs a second register write
		reg = this->checked_out.front();
		this->checked_out.pop_front();
		this->store_register(reg, this->curr_instr->get_s3());
	}

	this->checked_out.pop_front();
	reg = this->curr_instr->get_checked_out();
	this->store_register(reg, this->curr_instr->get_s1());

}

void WB::jump_handler()
{
	if (this->curr_instr->get_s1() > 0) {
		if (this->curr_instr->get_mnemonic() == JAL)
			this->gprs[1] = this->curr_instr->get_pc() + 1;;
		this->pc = this->curr_instr->get_s1();
		this->checked_out = {};
		this->next->squash();
	}
}

bool WB::should_jump()
{
	Type t;

	t = this->curr_instr->get_type();
	return t == J;
}
