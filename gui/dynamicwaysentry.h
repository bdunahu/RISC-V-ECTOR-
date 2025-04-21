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

#ifndef DYNAMICWAYSENTRY_H
#define DYNAMICWAYSENTRY_H


#include <QLineEdit>
#include <QStringList>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

class DynamicWaysEntry : public QWidget
{
  public:
	DynamicWaysEntry(QWidget *parent = nullptr);
	QStringList get_entries() const;
	/**
	 * Parses a string from this entry field, if it is valid.
	 * @param a string
	 * @param -1 if the string is not suitable as a way, an integer compatible
	 * with the cache constructor otherwise.
	 */
	int parse_valid_way(QString t);
  private slots:
	void on_number_enter(const QString &t);

  private:
	QVBoxLayout *l;
	QVector<QLineEdit *> fields;
	QStringList entries;
	void add_field();
	void remove_last_field();
};

#endif // DYNAMICWAYSENTRY_H
