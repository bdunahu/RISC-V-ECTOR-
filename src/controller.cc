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

#include "definitions.h"
#include "controller.h"
#include "ex.h"
#include "response.h"
#include "storage.h"

Controller::Controller(Stage *stage, Storage *storage, bool is_pipelined)
	: Stage(stage)
{
	this->clock_cycle = 1;
	this->storage = storage;
	this->is_pipelined = is_pipelined;
	this->is_empty = true;
	this->pc = 0x0;
	this->checked_out = {};
	this->gprs = {0};
	this->vrs.fill({0});
	this->vrs.at(0).fill(1);      // constant 1 vector register
	this->gprs.at(2) = MEM_WORDS; // set the stack pointer
}

void Controller::run_for(long number)
{
	int i;
	for (i = 0; i < number; ++i) {
		try {
			this->advance(READY);
		} catch (HaltException &e) {
			break;
		}
	}
}

int Controller::get_clock_cycle() { return this->clock_cycle; }

std::array<signed int, GPR_NUM> Controller::get_gprs() { return this->gprs; }

std::array<std::array<signed int, V_R_LIMIT>, V_NUM> Controller::get_vrs()
{
	return this->vrs;
}

int Controller::get_pc() { return this->pc; }

InstrDTO *Controller::advance(Response p)
{
	InstrDTO *r;
	r = this->next->advance(p);
	++this->clock_cycle;

	if (r)
		this->is_empty = true;

	return r;
}

void Controller::advance_helper()
{
	// TODO: check halt condition and call UI to refresh
}
