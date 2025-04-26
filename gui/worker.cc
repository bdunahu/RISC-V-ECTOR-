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

Worker::Worker(QObject *parent) : QObject(parent) {}

Worker::~Worker()
{
	emit finished();
	qDebug() << "Worker destructor called in thread:"
			 << QThread::currentThread();
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
	if (ways.size() != 0) {
		// TODO optimal proper sizes
		this->size_inc = ((MEM_LINE_SPEC * 0.75) / ways.size());
	}
	d = new Dram(DRAM_DELAY);
	s = static_cast<Storage *>(d);

	this->s.push_front(s);
	d->load(program);

	for (i = ways.size(); i > 0; --i) {
		s = static_cast<Storage *>(new Cache(
			s, this->size_inc * (i), ways.at(i - 1), CACHE_DELAY + i));
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
	qDebug() << "Running for " << steps << "steps";
	this->ct->run_for(steps);
	this->update();
}

void Worker::update()
{
	unsigned long i;

	this->ct_mutex.lock();
	emit register_storage(this->ct->get_gprs());

	emit storage(this->s.at(0)->view(0, 255), 1);

	for (i = 1; i < s.size(); ++i)
		emit storage(
			this->s.at(i - 1)->view(0, 1 << this->size_inc * i), i + 1);

	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit if_info(this->if_stage->get_instr());
	emit id_info(this->id_stage->get_instr());
	emit ex_info(this->ex_stage->get_instr());
	emit mm_info(this->mm_stage->get_instr());
	emit wb_info(this->wb_stage->get_instr());
	this->ct_mutex.unlock();
}
