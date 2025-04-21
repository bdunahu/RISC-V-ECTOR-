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
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

MM::MM(Stage *stage) : Stage(stage) { this->id = MEM; }

void MM::advance_helper()
{
	signed int data;
	int i;

	switch (this->curr_instr->get_mnemonic()) {
	case LOAD:
		i = this->storage->read_word(this, this->curr_instr->get_s1(), data);
		this->status = i ? OK : STALLED;
		if (this->status == OK) {
			this->curr_instr->set_s1(data);
		} else
			this->status = STALLED;
		break;

	case PUSH:
	case STORE:
		i = this->storage->write_word(
			this, this->curr_instr->get_s2(), this->curr_instr->get_s1());
		this->status = i ? OK : STALLED;
		if (this->status != OK) {
			this->status = STALLED;
		}
		break;

	case POP:
		i = this->storage->read_word(this, this->curr_instr->get_s3(), data);
		this->status = i ? OK : STALLED;
		if (this->status == OK) {
			this->curr_instr->set_s3(data);
		} else
			this->status = STALLED;
		break;

	default:
		this->status = OK;
	}
}
