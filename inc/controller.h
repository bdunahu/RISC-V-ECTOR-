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

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

/**
 * Houses the clock, and acts as the main API to the GUI.
 */
class Controller : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The stage(s) to interface with.
	 * @param The storage object to use.
	 * @param Whether or not efficient pipelining will be used.
	 * @return A newly allocated controller object.
	 */
	Controller(Stage *stage, Storage *storage, bool is_pipelined);
	InstrDTO *advance(Response p) override;

	/**
	 * Direct the simulator to run for `number` clock cycles.
	 * @param the number of clock cycles to run for.
	 */
	void run_for(long number);
	/**
	 * @return the current clock cycle.
	 */
	int get_clock_cycle();
	/**
	 * @return a copy of gprs.
	 */
	std::array<signed int, GPR_NUM> get_gprs();
	/**
	 * @return a copy of vrs.
	 */
	std::array<std::array<signed int, V_R_LIMIT>, V_NUM> get_vrs();
	/**
	 * @return the pc.
	 */
	int get_pc();

  private:
	void advance_helper() override;
};

#endif /* CONTROLLER_H_INCLUDED */
