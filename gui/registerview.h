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

#ifndef REGISTERVIEW_H
#define REGISTERVIEW_H

#include "storageview.h"
#include <QAbstractTableModel>
#include <QVector>

// see https://doc.qt.io/qt-6/qabstracttablemodel.html
class RegisterView : public StorageView
{
	Q_OBJECT
  public:
	using StorageView::StorageView;

	/**
	 * Returns a properly formatted cell, including alignment.This function is
	 * specific to the implementation details of QAbstractTableModel.
	 */
	QVariant
	data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	/**
	 * Adds custom formatting options for row and column headers.
	 */
	QVariant headerData(
		int section,
		Qt::Orientation o,
		int role = Qt::DisplayRole) const override;

	/**
	 * @param field to assign to `this->gprs'.
	 * @param field to assign to `this->vrs'.
	 */
	void set_data(const QVector<int> &gprs, const QVector<QVector<int>> &vrs);

  private:
	/**
	 * The general purpose registers.
	 */
	QVector<int> gprs;
	/**
	 * The vector registers.
	 */
	QVector<QVector<int>> vrs;
};

#endif // REGISTERVIEW_H
