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

#include "mm.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

void MM::advance_helper()
{
	signed int data;
	int i;

	this->try_start();
	if (this->status == OK) {
		return;
	}

	switch (this->curr_instr->mnemonic) {
	case LOAD:
		i = this->storage->read_word(
			this, this->curr_instr->operands.integer.slot_one, data);
		if (i) {
			this->curr_instr->operands.integer.slot_one = data;
		}
		this->try_finish(i);
		break;
	case SRDL:
		i = this->storage->read_word(
			this,
			this->curr_instr->operands.s_vector.slot_one[this->curr_element],
			data);
		if (i) {
			this->curr_instr->operands.s_vector.slot_one[this->curr_element] =
				data;
		}
		this->try_finish(i);
		break;

	case PUSH:
	case STORE:
		i = this->storage->write_word(
			this, this->curr_instr->operands.integer.slot_two,
			this->curr_instr->operands.integer.slot_one);
		this->try_finish(i);
		break;
	case SRDS:
		i = this->storage->write_word(
			this,
			this->curr_instr->operands.s_vector.slot_three[this->curr_element],
			this->curr_instr->operands.s_vector.slot_one[this->curr_element]);
		this->try_finish(i);
		break;

	case POP:
		i = this->storage->read_word(
			this, this->curr_instr->operands.integer.slot_three, data);
		this->status = i ? OK : STALLED;
		if (i) {
			this->curr_instr->operands.integer.slot_three = data;
		}
		break;

	default:
		this->status = OK;
	}
}

void MM::try_start()
{
	if (this->curr_instr->type != SI_INT) {
		this->status =
			(this->curr_element >= this->curr_instr->slot_B) ? OK : STALLED;
	}
	if (this->status == OK)
		this->curr_element = 0;
}

void MM::try_finish(int response)
{
	if (this->curr_instr->type == SI_INT) {
		this->status = response ? OK : STALLED;
	} else {
		if (response) {
			++this->curr_element;
		}
	}
}
