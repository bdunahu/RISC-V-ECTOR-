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

#ifndef STORAGEVIEW_H
#define STORAGEVIEW_H

#include <QAbstractTableModel>
#include <QVector>

// see https://doc.qt.io/qt-6/qabstracttablemodel.html
class StorageView : public QAbstractTableModel
{
	Q_OBJECT
  public:
	/**
	 * Constructor. Initializes a clean StorageView object with
	 * `rows' rows.
	 * @param the number of rows
	 */
	StorageView(int rows, int columns, QObject *parent = nullptr);

	/**
	 * Returns the number of rows in this table.
	 * @param the parent
	 * @return the number of rows under the given parent.
	 */
	int rowCount(const QModelIndex &) const override;
	/**
	 * Returns the number of columns in this table.
	 * @param the parent
	 * @return the number of columns under the given parent (hint: it's
	 * LINE_SIZE)
	 */
	int columnCount(const QModelIndex &) const override;

	/**
	 * Returns a properly formatted cell, including alignment.This function is
	 * specific to the implementation details of QAbstractTableModel.
	 */
	virtual QVariant
	data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	/**
	 * Adds custom formatting options for row and column headers.
	 */
	virtual QVariant headerData(
		int section,
		Qt::Orientation o,
		int role = Qt::DisplayRole) const override;

	/**
	 * Ensures the table is enabled, but not selectable.
	 */
	Qt::ItemFlags flags(const QModelIndex &i) const override;

	/**
	 * @param field to assign to `this->d'
	 */
	void set_data(const QVector<QVector<int>> &data);

  public slots:
	void set_hex_display(bool hex);

  protected:
	/**
	 * The number of rows in this table.
	 */
	int r;
	/**
	 * The number of columns in this table.
	 */
	int c;
	/**
	 * Whether or not the headers should be displayed in hex.
	 */
	bool is_hex = true;

  private:
	/**
	 * The data this table displays.
	 */
	QVector<QVector<int>> d;
};

#endif // STORAGEVIEW_H
