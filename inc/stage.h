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

#ifndef STAGE_H
#define STAGE_H
#include "instrDTO.h"
#include "pipe_spec.h"
#include "response.h"
#include "storage.h"
#include <array>
#include <deque>
#include <memory>

enum CC {
	GT,
	EQ,
	UF,
	OF,
};

class Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated stage object.
	 */
	Stage(Stage *next);
	virtual ~Stage();
	/**
	 * Advances this stage by a single clock cycle.
	 * A boilerplate version is provided in stage.cc.
	 *
	 * @param a response, indicating whether or not the parent pipe stage is
	 * ready to accept a new instruction object next cycle.
	 * @return a DTO object containing the next instruction to be processed.
	 *
	 * Must set the status to READY when the current instruction is evicted..
	 */
	virtual InstrDTO *advance(Response p);
	/**
	 * @return the current instruction.
	 */
	InstrDTO *get_instr();
	/**
	 * Squashes the pipeline.
	 */
	void squash();

	/* The following methods are made public so that they may be tested, and are
	 * not to be called from outside classes during standard execution.
	 */

	/**
	 * Gets the bit in the condition code register correspondng to `c`.
	 * @param the condition code to retrieve,
	 */
	bool get_condition(CC c);
	/**
	 * Sets the bit in the condition code register corresponding to `c`.
	 * @param the condition code to set.
	 * @param the truthy value to set it to.
	 */
	void set_condition(CC c, bool v);

	/**
	 * The set of registers currently checked out.
	 */
	static std::deque<signed int> checked_out;

  protected:
	/**
	 * The function expected to do the majority of the work.
	 *
	 * Must set the status to OK when an operation is done.
	 * Must set the status to STALLED when an operation cannot be completed the
	 * current cycle.
	 */
	virtual void advance_helper() = 0;
	/**
	 * Helper for `check_out`.
	 * Returns true if r are not checked out, false otherwise.
	 * @param a list of register numbers.
	 * @return true if registers are not in checked_out, false otherwise.
	 */
	bool is_checked_out(signed int r);
	/**
	 * Stores `d` into the register indexed `v`.
	 * @param the register number.
	 * @param the value to store.
	 */
	template <typename T>
	void store_register(signed int v, T d)
	{
		if constexpr (std::is_same_v<T, signed int>) {
			if (v < 0 || v >= GPR_NUM) {
				throw std::out_of_range("Invalid GPR index for storing scalar");
			}
			gprs[v] = d;
		}
		else if constexpr (std::is_same_v<T, std::array<signed int, V_R_LIMIT>>) {
			if (v < GPR_NUM || v >= GPR_NUM + V_NUM) {
				throw std::out_of_range("Invalid VR index for storing vector");
			}
			vrs[v % GPR_NUM] = d;
		}
	}
	/**
	 * Returns the value of the register corresponding to `v`.
	 * @param the register number.
	 * @return the value in the associated register.
	 */
	template <typename T>
	T dereference_register(signed int v)
	{
		if constexpr (std::is_same_v<T, signed int>) {
			if (v < 0 || v >= GPR_NUM) {
				throw std::out_of_range("Invalid GPR index");
			}
			return gprs[v];
		}
		else if constexpr (std::is_same_v<T, std::array<signed int, V_R_LIMIT>>) {
			if (v < GPR_NUM || v >= GPR_NUM + V_NUM) {
				throw std::out_of_range("Invalid VRS index");
			}
			return vrs[v % GPR_NUM];
		}
	}
	/**
	 * The shared pool of general-purpose integer registers.
	 */
	static std::array<signed int, GPR_NUM> gprs;
	/**
	 * The shared pool of general-purpose vector registers.
	 */
	static std::array<std::array<signed int, V_R_LIMIT>, V_NUM> vrs;
	/**
	 * The address of the currently executing instruction.
	 */
	static unsigned int pc;
	/**
	 * A pointer to the top-level storage device.
	 */
	static Storage *storage;
	/**
	 * A flag indicating whether pipelining should be used.
	 */
	static bool is_pipelined;
	/**
	 * A flag which tells fetch when the pipe is empty. Only used when the pipe
	 * is turned off.
	 */
	static bool is_empty;
	/**
	 * The current clock cycle.
	 */
	static int clock_cycle;
	/**
	 * A pointer to the next stage in the pipeline.
	 */
	Stage *next;
	/**
	 * A pointer to the current instruction this stage is processing.
	 */
	InstrDTO *curr_instr;
	/**
	 * The current status of this stage.
	 */
	Response status;
};

#endif /* STAGE_H_INCLUDED */
