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

#include "digitlabel.h"

DigitLabel::DigitLabel(QWidget *parent) : QLabel(parent)
{
	this->update_display();
}

void DigitLabel::clear()
{
	this->is_cleared = true;
	setText(QString());
}

void DigitLabel::set_value(int v)
{
	this->is_cleared = false;
	if (this->v != v) {
		this->v = v;
		update_display();
	}
}

void DigitLabel::toggle_mode()
{
	this->is_hex = !this->is_hex;
	this->update_display();
}

void DigitLabel::update_display()
{
	QString t;
	if (this->is_cleared) {
		setText(QString());
	} else {
		t = (this->is_hex) ? QString::number(this->v, 6).toUpper()
						   : QString::number(this->v);
		setText(t);
	}
}
