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

Worker::Worker(QObject *parent) : QObject(parent) {}

void Worker::doWork()
{
	qDebug() << "Initializing...";
	this->d = new Dram(0);
	this->c = new Cache(this->d, 8, 0, 0);
	this->if_stage = new IF(nullptr);
	this->id_stage = new ID(if_stage);
	this->ex_stage = new EX(id_stage);
	this->mm_stage = new MM(ex_stage);
	this->wb_stage = new WB(mm_stage);
	this->ct = new Controller(wb_stage, this->c, true);

	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit dram_storage(this->d->view(0, 255));
	emit cache_storage(this->c->view(0, 255));
	emit register_storage(this->ct->get_gprs());
}

Worker::~Worker()
{
	emit finished();
	qDebug() << "Worker destructor called in thread:"
			 << QThread::currentThread();
	delete this->ct;
	delete this->c;
}

void Worker::loadProgram(std::vector<signed int> p) {
	this->d->load(p);
}

void Worker::refreshDram()
{
	qDebug() << "Refreshing Dram";
	emit dram_storage(this->d->view(0, 31));
}

void Worker::refreshCache()
{
	qDebug() << "Refreshing Dram";
	emit cache_storage(this->c->view(0, 255));
}

void Worker::refreshRegisters()
{
	qDebug() << "Refreshing Registers";
	emit register_storage(this->ct->get_gprs());
}

void Worker::runSteps(int steps)
{
	qDebug() << "Running for steps: " << steps;
	this->ct->run_for(steps);
	emit dram_storage(this->d->view(0, 255));
	emit cache_storage(this->c->view(0, 255));
	emit register_storage(this->ct->get_gprs());
	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit if_info(this->if_stage->stage_info());
	emit id_info(this->id_stage->stage_info());
	emit ex_info(this->ex_stage->stage_info());
	emit mm_info(this->mm_stage->stage_info());
	emit wb_info(this->wb_stage->stage_info());
}

void Worker::runStep()
{
	qDebug() << "Running for 1 step ";
	this->ct->run_for(1);
	emit dram_storage(this->d->view(0, 255));
	emit cache_storage(this->c->view(0, 255));
	emit register_storage(this->ct->get_gprs());
	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit if_info(this->if_stage->stage_info());
	emit id_info(this->id_stage->stage_info());
	emit ex_info(this->ex_stage->stage_info());
	emit mm_info(this->mm_stage->stage_info());
	emit wb_info(this->wb_stage->stage_info());
}
