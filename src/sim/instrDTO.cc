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

#include "instrDTO.h"
#include "accessor.h"

InstrDTO::InstrDTO()
{
	this->instr_bits = 0;
	this->checked_out = -1;
	this->s1 = 0;
	this->s2 = 0;
	this->s3 = 0;
	this->mnemonic = ADD;
	this->type = INV;
	this->pc = 0;
	this->squashed = 0;
}

signed int InstrDTO::get_instr_bits() { return this->instr_bits; }

signed int InstrDTO::get_checked_out() { return this->checked_out; }

signed int InstrDTO::get_s1() { return this->s1; }

signed int InstrDTO::get_s2() { return this->s2; }

signed int InstrDTO::get_s3() { return this->s3; }

Mnemonic InstrDTO::get_mnemonic() { return this->mnemonic; }

Type InstrDTO::get_type() { return this->type; }

unsigned int InstrDTO::get_pc() { return this->pc; }

int InstrDTO::is_squashed() { return this->squashed; }

void InstrDTO::set_instr_bits(signed int instr) { this->instr_bits = instr; }

void InstrDTO::set_checked_out(signed int checked_out)
{
	this->checked_out = checked_out;
}

void InstrDTO::set_s1(signed int s) { this->s1 = s; }

void InstrDTO::set_s2(signed int s) { this->s2 = s; }

void InstrDTO::set_s3(signed int s) { this->s3 = s; }

void InstrDTO::set_mnemonic(Mnemonic m) { this->mnemonic = m; }

void InstrDTO::set_type(Type t) { this->type = t; }

void InstrDTO::set_pc(unsigned int pc) { this->pc = pc; }

void InstrDTO::squash() { this->squashed = 1; }
