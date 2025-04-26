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

#include "if.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

InstrDTO *IF::advance(Response p)
{
	InstrDTO *r = nullptr;

	this->advance_helper();
	if (this->curr_instr != nullptr && p == READY) {
		// don't increment PC if the PC was just set by wb
		if (this->curr_instr->is_squashed != 1)
			++this->pc;
		r = new InstrDTO(*this->curr_instr);
		delete curr_instr;
		curr_instr = nullptr;
	}

	return r;
}

void IF::advance_helper()
{
	Response r;
	int i;
	signed int bits;

	if (this->curr_instr == nullptr && (this->is_pipelined || this->is_empty)) {
		i = this->storage->read_word(this, this->pc, bits);
		r = i ? OK : STALLED;
		if (r == OK) {
			this->curr_instr = new InstrDTO();
			this->curr_instr->slot_A = bits;
			this->curr_instr->slot_B = this->pc;
			this->curr_instr->is_squashed = 0;
			this->curr_instr->checked_out = -1;
			this->curr_instr->mnemonic = ADD;
			this->is_empty = false;
		}
	}
}
