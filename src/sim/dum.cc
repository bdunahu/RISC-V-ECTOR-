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

#include "dum.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

DUM::DUM(Stage *stage) : Stage(stage) { this->id = IDLE; }

InstrDTO *DUM::advance(Response p)
{
	InstrDTO *r = nullptr;

	if (this->curr_instr && p == WAIT) {
		r = new InstrDTO(*this->curr_instr);
		delete this->curr_instr;
		curr_instr = nullptr;
	}

	return r;
}

void DUM::advance_helper() {}

void DUM::set_curr_instr(InstrDTO *d)
{
	this->curr_instr = d;
}
