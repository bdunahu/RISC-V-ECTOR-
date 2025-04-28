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

#ifndef INSTRDTO_H
#define INSTRDTO_H
#include "instr.h"
#include "pipe_spec.h"
#include<array>

struct U_INT_TYPE {
	signed int slot_one;
	signed int slot_two;
	signed int slot_three;
};

struct V_TYPE {
	std::array<signed int, V_R_LIMIT> slot_one;
	std::array<signed int, V_R_LIMIT> slot_two;
	std::array<signed int, V_R_LIMIT> slot_three;
};

struct LOAD_STORE_V_TYPE{
	signed int base_addr;
	signed int immediate;
	std::array<signed int, V_R_LIMIT> vector_register;
};

struct InstrDTO {
	/**
	 * If this instruction is squashed or not.
	 */
	unsigned int is_squashed : 1;
	/**
	 * Optional slot for holding the Instruction Bits
	 */
	signed int slot_A;
	/**
	 * Optional slot for holding PC
	 */
	signed int slot_B;
	/**
	 * The mnemonic of the instruction.
	 */
	Mnemonic mnemonic;
	/**
	 * The register this instruction checks out.
	 */
	signed int checked_out;
	union {
		struct U_INT_TYPE integer;
		struct V_TYPE vector;
		struct LOAD_STORE_V_TYPE load_store_vector;
	} operands;
};

#endif /* INSTRDTO_H_INCLUDED */
