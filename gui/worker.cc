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

#include "worker.h"
#include "storage.h"
#include "util.h"

Worker::Worker(QObject *parent) : QObject(parent) {}

Worker::~Worker()
{
	emit finished();
	delete this->ct;
}

void Worker::configure(
	std::vector<unsigned int> ways,
	std::vector<signed int> program,
	bool is_pipelined)
{
	Dram *d;
	Storage *s;
	Stage *old;
	int i;

	this->s.clear();

	this->ct_mutex.lock();
	d = new Dram(DRAM_DELAY);
	s = static_cast<Storage *>(d);

	this->s.push_front(s);
	d->load(program);

	for (i = ways.size(); i > 0; --i) {
		s = static_cast<Storage *>(new Cache(
			s, cache_size_mapper(ways.size() - 1, i - 1), ways.at(i - 1),
			CACHE_DELAY + i));
		this->s.push_front(s);
	}

	this->if_stage = new IF(nullptr);
	this->id_stage = new ID(if_stage);
	this->ex_stage = new EX(id_stage);
	this->mm_stage = new MM(ex_stage);
	this->wb_stage = new WB(mm_stage);

	old = static_cast<Stage *>(this->ct);
	this->ct = new Controller(wb_stage, s, is_pipelined);
	if (old)
		delete old;
	this->ct_mutex.unlock();

	this->update();
}

void Worker::runSteps(int steps)
{
	this->ct->run_for(steps);
	this->update();
	emit steps_done();
}

void Worker::update()
{
	unsigned long i;
	std::array<int, GPR_NUM> gprs;
	std::array<std::array<signed int, V_R_LIMIT>, V_NUM> vrs;

	this->ct_mutex.lock();
	gprs = this->ct->get_gprs();
	vrs = this->ct->get_vrs();
	std::vector<std::array<signed int, V_R_LIMIT>> v(vrs.begin(), vrs.end());

	emit register_storage(
		QVector<int>(gprs.begin(), gprs.end()), this->data_to_QT(v));

	for (i = 0; i < s.size(); ++i)
		emit storage(this->data_to_QT(this->s.at(i)->get_data()), i + 1);

	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit if_info(this->if_stage->get_instr());
	emit id_info(this->id_stage->get_instr());
	emit ex_info(this->ex_stage->get_instr());
	emit mm_info(this->mm_stage->get_instr());
	emit wb_info(this->wb_stage->get_instr());
	this->ct_mutex.unlock();
}
