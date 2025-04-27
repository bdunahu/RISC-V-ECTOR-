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
	int vector_delay = VECTOR_MEM_DELAY;

	switch (this->curr_instr->mnemonic) {
	case LOAD:
		i = this->storage->read_word(
			this, this->curr_instr->operands.integer.slot_one, data);
		this->status = i ? OK : STALLED;
		if (this->status == OK) {
			this->curr_instr->operands.integer.slot_one = data;
		} else
			this->status = STALLED;
		break;
	case LOADV:
		if (vector_delay == 0){
			signed int word_address = this->curr_instr->operands.load_store_vector.base_addr;
			int j = 0;
			while(j < this->curr_instr->slot_A){
				i = this->storage->read_word(this, word_address, data);
				this->status = i ? OK : STALLED;
				if (this->status == OK) {
					this->curr_instr->operands.load_store_vector.vector_register[j] = data;
					// +1 or +4?
					word_address += 1;
					j++;
				} else {
					break;
				}
			}
			if(this->status == OK){
				// if vector is loaded, reset delay
				vector_delay = VECTOR_MEM_DELAY;
			} else {
				this->status = STALLED;
			}
		} else {
			vector_delay--;
		}
		break;

	case PUSH:
	case STORE:
		i = this->storage->write_word(
			this, this->curr_instr->operands.integer.slot_two,
			this->curr_instr->operands.integer.slot_one);
		this->status = i ? OK : STALLED;
		if (this->status != OK) {
			this->status = STALLED;
		}
		break;
	case STOREV:
	if (vector_delay == 0){
		signed int word_address = this->curr_instr->operands.load_store_vector.base_addr;
		int j = 0;
		while(j < this->curr_instr->slot_A){
			this->storage->write_word(
				this, this->curr_instr->operands.load_store_vector.vector_register[j], word_address);
			this->status = i ? OK : STALLED;
			if (this->status != OK) {
				break;
			} else {
				word_address += 1;
				j++;
			}
		}
		if(this->status == OK){
			// if vector is stored , reset delay
			vector_delay = VECTOR_MEM_DELAY;
		} else {
			this->status = STALLED;
		}
	} else {
		vector_delay--;
	}
	break;

	case POP:
		i = this->storage->read_word(this, this->curr_instr->operands.integer.slot_three, data);
		this->status = i ? OK : STALLED;
		if (this->status == OK) {
			this->curr_instr->operands.integer.slot_three = data;
		} else
			this->status = STALLED;
		break;

	default:
		this->status = OK;
	}
}
