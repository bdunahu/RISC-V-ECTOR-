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

#include "stage.h"
#include "pipe_spec.h"
#include <array>
#include <deque>

Stage::Stage(Stage *next)
{
	this->next = next;
	this->curr_instr = nullptr;
	this->status = READY;
}

Stage::~Stage() { delete this->next; };

std::array<int, GPR_NUM> Stage::gprs;
std::array<std::array<signed int, V_R_LIMIT>, V_NUM> Stage::vrs;
std::deque<signed int> Stage::checked_out;
unsigned int Stage::pc;
Storage *Stage::storage;
bool Stage::is_pipelined;
bool Stage::is_empty;
int Stage::clock_cycle;

bool Stage::get_condition(CC c) { return (this->gprs[3] >> c) & 1; }

InstrDTO *Stage::advance(Response p)
{
	InstrDTO *r = nullptr;
	InstrDTO *s = nullptr;
	Response n;

	if (this->curr_instr && this->curr_instr->is_squashed == 1)
		this->status = OK;
	if (this->curr_instr && this->status != OK) {
		this->advance_helper();
	}
	if (this->status == OK && p == READY && this->curr_instr) {
		// mutual consent
		r = new InstrDTO(*this->curr_instr);
		delete curr_instr;
		curr_instr = nullptr;
		this->status = READY;
	}

	n = (p != READY || this->status != READY) ? STALLED : READY;
	s = this->next->advance(n);
	if (s)
		this->curr_instr = s;
	return r;
}

bool Stage::is_vector_type(Mnemonic m)
{
	return (
		m == ADDV || m == SUBV || m == MULV || m == DIVV || m == CEV ||
		m == LOADV || m == STOREV);
}

bool Stage::is_logical(Mnemonic m)
{
	return (
		m == ANDI || m == ORI || m == XORI || m == AND || m == OR || m == XOR ||
		m == NOT);
}

InstrDTO *Stage::get_instr() { return this->curr_instr; }

void Stage::set_condition(CC c, bool v)
{
	if (v)
		this->gprs[3] = this->gprs[3] | 1 << c;
	else
		this->gprs[3] = this->gprs[3] & ~(1 << c);
}

bool Stage::is_checked_out(signed int r)
{
	return std::find(this->checked_out.begin(), this->checked_out.end(), r) !=
		   this->checked_out.end();
}

void Stage::squash()
{
	if (curr_instr) {
		this->curr_instr->is_squashed = 1;
		this->status = OK;
	}
	if (this->next) {
		this->next->squash();
	}
}
