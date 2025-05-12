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

#ifndef WB_H
#define WB_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class WB : public Stage
{
  public:
	using Stage::advance;
	using Stage::Stage;

  private:
	void advance_helper() override;
	/**
	 * Performs the actual work of storing into a register.
	 */
	void write_handler();
	/**
	 * Performs the actual work of processing a jump instruction.
	 */
	void jump_handler();
	/**
	 * @return true if the current instruction is an R or I type and is not a
	 * STORE.
	 */
	bool should_write();
	/**
	 * @return true if the current instruction is a J type and is not a push.
	 * STORE.
	 */
	bool should_jump();
	/**
	 * @return the vector register to be stored, obtained by copying the
	 * unfilled elements in the destination register to the source. This is
	 * required to ensure what is written back only changes VECTOR_LENGTH number
	 * of elements. Correctly handles zeroing out ALU operations if the VECTOR
	 * LENGTH number is zero.
	 */
	std::array<signed int, V_R_LIMIT> copy_extra_vector_elements();
};

#endif /* WB_H_INCLUDED */
