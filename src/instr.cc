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

#include "instr.h"
#include <functional>
#include <map>
#include <unordered_map>

namespace instr
{
const std::unordered_map<unsigned int, Mnemonic> mnemonic_map = {
	{0b0000100, ADD},	{0b0001000, SUB},	{0b0001100, MUL},
	{0b0010000, QUOT},	{0b0010100, REM},	{0b0011000, SFTR},
	{0b0011100, SFTL},	{0b0100000, AND},	{0b0100100, OR},
	{0b0101000, NOT},	{0b0101100, XOR},	{0b0110000, ADDV},
	{0b0110100, SUBV},	{0b0111000, MULV},	{0b0111100, DIVV},
	{0b1000000, CMP},	{0b1000100, CEV},	{0b1001000, SRDL},
	{0b1001100, SRDS},	{0b1010000, ROTV},	{0b000101, LOAD},
	{0b0001001, ADDI},	{0b0001101, SUBI},	{0b0010001, SFTRI},
	{0b0010101, SFTLI}, {0b0011001, ANDI},	{0b0011101, ORI},
	{0b0100001, XORI},	{0b0100101, STORE}, {0b0000110, JMP},
	{0b0001010, JRL},	{0b0001110, JAL},	{0b0010010, BEQ},
	{0b0010110, BGT},	{0b0011010, BUF},	{0b0011110, BOF},
	{0b0100010, PUSH},	{0b0100110, POP},	{0b0101010, RET},
};

FieldType get_field_types(Mnemonic m)
{
	if (m == ADDV || m == SUBV || m == MULV || m == DIVV || m == CEV) {
		return R_VECT;
	} else if (m == SRDL || m == SRDS || m == ROTV) {
		return S_VECT;
	} else {
		return SI_INT;
	}
}
} // namespace instr
