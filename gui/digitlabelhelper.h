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

#ifndef DIGITLABELHELPER_H
#define DIGITLABELHELPER_H

#include <QString>

class DigitLabelHelper
{
  public:
	/**
	 * Contains the main formatting logic used to format integers.
	 * @param the value to be formated
	 * @param if the value should be displayed in hex. If false, displays in
	 * decimal.
	 * @param if the value should display.
	 * @return a string respecting the above parameters.
	 */
	static QString format_value(int value, bool is_hex, bool is_cleared = false)
	{
		if (is_cleared)
			return QString();
		return is_hex ? QString::number(value, 16).toUpper()
					  : QString::number(value);
	}
};

#endif // DIGITLABELHELPER_H
