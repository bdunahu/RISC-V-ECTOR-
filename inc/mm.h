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

#ifndef MM_H
#define MM_H
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

class MM : public Stage
{
  public:
	using Stage::Stage;
	using Stage::advance;

  private:
	void advance_helper() override;
	/**
	 * Helpers for `advance_helper'. Sets the `this->status' to OK
	 * If the current memory IO returned OK, and all vector elements
	 * have been processed. Otherwise, sets `this->status' to STALLED
	 * @param the response from the storage devices.
	 */
	void try_start();
	void try_finish(int response);
	/**
	 * The index element currently being loaded or stored.
	 * Used for strided load/store.
	 */
	int curr_element = 0;
};

#endif /* MM_H_INCLUDED */
