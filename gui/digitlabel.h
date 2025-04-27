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

#ifndef DIGITLABEL_H
#define DIGITLABEL_H

#include <QLabel>

class DigitLabel : public QLabel
{
	Q_OBJECT

  public:
	/**
	 * Constructor.
	 * @return a newly allocated DigitLabel.
	 */
	explicit DigitLabel(QWidget *parent);

	/**
	 * Sets the empty flag.
	 */
	void clear();
	/**
	 * @param the value to set `this->v' with.
	 */
	void set_value(int v);

  public slots:
	/**
	 * Toggles the base this label displays in, by setting `this->is_hex'.
	 */
	void on_hex_toggle(bool is_hex);

  private:
	/**
	 * Refreshes the display of this label, taking base into consideration..
	 */
	void update_display();

	/**
	 * The decimal value associated with this label.
	 */
	int v;
	/**
	 * If this digit should display in hexidecinmal.
	 */
	int is_hex = true;
	/**
	 * If this digit should not display.
	 */
	bool is_cleared = true;
};

#endif // DIGITLABEL_H
