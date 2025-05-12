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

#ifndef INSTR_H
#define INSTR_H
#include <unordered_map>

enum Mnemonic {
	ADD,
	SUB,
	MUL,
	QUOT,
	REM,
	SFTR,
	SFTL,
	AND,
	OR,
	NOT,
	XOR,
	ADDV,
	SUBV,
	MULV,
	DIVV,
	CMP,
	CEV,
	SRDL,
	SRDS,
	ROTV,
	LOAD,
	ADDI,
	SUBI,
	SFTRI,
	SFTLI,
	ANDI,
	ORI,
	XORI,
	STORE,
	JMP,
	JRL,
	JAL,
	BEQ,
	BGT,
	BUF,
	BOF,
	PUSH,
	POP,
	RET,
	NOP,
};

enum FieldType {
	SI_INT,
	R_VECT,
	S_VECT,
};

namespace instr
{
extern const std::unordered_map<unsigned int, Mnemonic> mnemonic_map;
/**
 * @param a mnemonic
 * @return an enum representing the types of the decoded instruction fields.
 */
FieldType get_field_types(Mnemonic m);
} // namespace instr

#endif /* INSTR_H_INCLUDED */
