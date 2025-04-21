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

#ifndef DUM_H
#define DUM_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

/**
 * Don't underestimate mocks (the DUM pipe stage).
 */
class DUM : public Stage
{
  public:
	/**
	 * Constructor.
	 * @param The next stage in the pipeline.
	 * @return A newly allocated DUM object.
	 */
	DUM(Stage *next);

	InstrDTO *advance(Response p) override;

	void set_curr_instr(InstrDTO *);

  private:
	void advance_helper() override;
};

#endif /* DUM_H_INCLUDED */
