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

#ifndef WORKER_H
#define WORKER_H

#include "cache.h"
#include "controller.h"
#include "dram.h"
#include "ex.h"
#include "id.h"
#include "if.h"
#include "mm.h"
#include "wb.h"
#include <QDebug>
#include <QMutex>
#include <QObject>
#include <QThread>
#include <deque>

class Worker : public QObject
{
	Q_OBJECT

  private:
	/**
	 * The storage objects, stored smallest to largest.
	 */
	std::deque<Storage *> s;
	IF *if_stage;
	ID *id_stage;
	EX *ex_stage;
	MM *mm_stage;
	WB *wb_stage;

	Controller *ct = nullptr;
	QMutex ct_mutex;

  public:
	explicit Worker(QObject *parent = nullptr);
	~Worker();
	QMutex &get_ct_mutex() { return ct_mutex; }

  public slots:
	void runSteps(int steps);
	void configure(
		std::vector<unsigned int> ways,
		std::vector<signed int> program,
		bool is_pipelined);

  signals:
	void clock_cycles(int value, int pc);
	void storage(QVector<QVector<int>> data, int i);
	void register_storage(
		QVector<signed int> gprs, QVector<QVector<signed int>> vrs);
	void if_info(const InstrDTO *);
	void id_info(const InstrDTO *);
	void ex_info(const InstrDTO *);
	void mm_info(const InstrDTO *);
	void wb_info(const InstrDTO *);
	void steps_done();
	void finished();

  private:
	/**
	 * Converts a vector of arrays into a QVector of QVectors.
	 * @param the original data
	 * @return a less universal version of the same thing
	 */
	template <size_t N>
	QVector<QVector<int>>
	data_to_QT(const std::vector<std::array<signed int, N>> &data)
	{
		QVector<QVector<int>> r;
		QVector<int> tmp;

		r.reserve(static_cast<int>(data.size()));

		for (const auto &line : data) {
			tmp = QVector<int>(line.begin(), line.end());
			r.append(tmp);
		}
		return r;
	}
	/**
	 * Sets the GUI signals to update the storage, clock cycle, and stage
	 * displays.
	 */
	void update();
};

#endif // WORKER_H
