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
#include <functional>
#include <string>
#include <unordered_map>

class InstrDTO
{
  public:
	/**
	 * Constructor.
	 */
	InstrDTO();
	~InstrDTO() = default;

	int get_id_cycle();
	/**
	 * @return instr_bits
	 */
	signed int get_instr_bits();
	/**
	 * @return checked_out
	 */
	signed int get_checked_out();
	/**
	 * @return s1
	 */
	signed int get_s1();
	/**
	 * @return s2
	 */
	signed int get_s2();
	/**
	 * @return s3
	 */
	signed int get_s3();
	/**
	 * @return the mnemonic of the instruction
	 */
	Mnemonic get_mnemonic();
	/**
	 * @return the type of the instruction
	 */
	Type get_type();
	/**
	 * @return the program counter at the time this instruction was fetched
	 */
	unsigned int get_pc();
	/**
	 * @return 1 if this instruction is invalid, 0 otherwise
	 */
	int is_squashed();

	/**
	 * @param instr_bits
	 */
	void set_instr_bits(signed int);
	/**
	 * @param checked_out
	 */
	void set_checked_out(signed int);
	/**
	 * @param s1
	 */
	void set_s1(signed int);
	/**
	 * @param s2
	 */
	void set_s2(signed int);
	/**
	 * @param s3
	 */
	void set_s3(signed int);
	/**
	 * @param the mnemonic of the instruction
	 */
	void set_mnemonic(Mnemonic);

	/**
	 * @param the type of the instruction
	 */
	void set_type(Type);
	/**
	 * @param the program counter at the time this instruction was fetched
	 */
	void set_pc(unsigned int pc);
	/**
	 * squashes this instruction
	 */
	void squash();

  private:
	/**
	 * The raw bits encoding the instruction.
	 */
	signed int instr_bits;
	/**
	 * The register, if any, this instruction has checked out. A checked out
	 * register cannot be checked out by another register. This prevents RAW
	 * conflicts.
	 */
	signed int checked_out;
	/**
	 * Slots in this instruction, for storing temporary registers, immediates,
	 * or other.
	 * Some instruction types may use these differently.
	 * The `oper` function is in charge of knowing how to parse these.
	 */
	signed int s1;
	signed int s2;
	signed int s3;
	/**
	 * The mnemonic of the operation.
	 */
	Mnemonic mnemonic;
	/**
	 * Type of the instruction
	 */
	Type type;
	/**
	 * The PC of the instruction
	 */
	unsigned int pc;
	/**
	 * If this instruction was made dead
	 */
	unsigned int squashed;
};

#endif /* INSTRDTO_H_INCLUDED */
