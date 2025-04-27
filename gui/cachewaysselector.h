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

#ifndef CACHEWAYSSELECTOR_H
#define CACHEWAYSSELECTOR_H

#include <QSpinBox>
#include <QWidget>

class CacheWaysSelector : public QWidget
{
	Q_OBJECT

  public:
	/**
	 * Constructor.
	 * This class provides a simple group of labeled spinboxs meant for
	 * selecting cache ways.
	 * @param The parent widget.
	 * @param a newly allocated CacheWaysSelector
	 */
	explicit CacheWaysSelector(QWidget *parent = nullptr);

	/**
	 * @return the values in the spinboxes.
	 */
	QList<int> values() const;

  private:
	/**
	 * A list of spinboxes.
	 */
	QList<QSpinBox *> sbs;
};

#endif // CACHEWAYSSELECTOR_H
