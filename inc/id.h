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

#ifndef ID_H
#define ID_H
#include "instr.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class ID : public Stage
{
  public:
	using Stage::Stage;
	using Stage::advance;

	/* The following methods are made public so that they may be tested, and are
	 * not to be called from outside classes during standard execution.
	 */

	/**
	 * Facilitates register checkout and data hazard management.
	 * It does this by checking that the register passed in is not currently
	 * checked out. If true, then replaces r with the value of the register and
	 * returns OK. If false, returns STALLED.
	 *
	 * @param the registers number, to be dereferenced.
	 * @return OK if `r` is not checked out, STALLED otherwise.
	 */
	Response read_guard(signed int &r);
	/**
	 * Facilitates register checkout and data hazard management.
	 * Checks out a register and returns it.
	 *
	 * @param the registers number, to be dereferenced and checked out.
	 */
	void write_guard(signed int &r);

	Response read_vec_guard(signed int r, std::array<signed int, V_R_LIMIT> &v);

	void write_vec_guard(signed int r, std::array<signed int, V_R_LIMIT> &v);

	Response set_vlen();

  private:
	/**
	 * Helper for `get_instr_fields`
	 * Attempts to parse and dereference instruction arguments. Uses read and
	 * write guards to prevent RAW conflicts.
	 *
	 * @param the resulting first field. To call this function properly, this
	 * field must contain the section of the instruction to be parsed.
	 * @param the resulting second field.
	 * @param the resulting third field.
	 */
	void advance_helper() override;
	/**
	 * Parse an instruction into a type, opcode, and fields. If the type is
	 * invalid, only the type field will be set.
	 *
	 * This method is marked public so it may be tested, and is not used outside
	 * of this class during normal execution.
	 *
	 * @param the resulting first field, which varies per type. To call this
	 * function properly, this field must contain the full instruction bytes on
	 * function entry.
	 * @param the resulting second field, which varies per type.
	 * @param the resulting third field, which varies per type.
	 * @param the resulting mnemonic.
	 */
	void get_instr_fields(signed int &s1);
	void decode_R_type(signed int &s1);
	void decode_I_type(signed int &s1);
	void decode_J_type(signed int &s1);
	/**
	 * Helper for `get_instr_fields`.
	 * Given a raw instruction, returns the mnemonic and type.
	 * This operation will destroy the original arguments.
	 * @param the raw bits to parse.
	 * @param the resulting mnemonic.
	 * @param the resulting type.
	 */
	void split_instr(signed int &raw, unsigned int &type, Mnemonic &m);
};

#endif /* ID_H_INCLUDED */
