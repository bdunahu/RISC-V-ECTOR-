#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent) {}

void Worker::doWork()
{
	qDebug() << "Initializing...";
	this->d = new Dram(0);
	this->c = new Cache(this->d, 5, 0, 0);
	this->if_stage = new IF(nullptr);
	this->id_stage = new ID(if_stage);
	this->ex_stage = new EX(id_stage);
	this->mm_stage = new MM(ex_stage);
	this->wb_stage = new WB(mm_stage);
	this->ct = new Controller(wb_stage, this->c, true);

	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit dram_storage(this->d->view(0, 32));
	emit cache_storage(this->c->view(0, 7));
	emit register_storage(this->ct->get_gprs());

	signed int b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14,
		b15, b16, b17, b18, b19, b20;
	std::vector<signed int> p;

	// I-TYPE              /    /    /   /
	b0 = 0b00000010000000000001000000001101;
	b1 = 0b00000000000000010010100000001101;
	b2 = 0b00000000000000000010100010101001;
	b3 = 0b00000000000000100010100000001101;
	b4 = 0b00000000000000010010100010101001;
	b5 = 0b00000000000000110010100000001101;
	b6 = 0b00000000000000100010100010101001;
	b7 = 0b00000000000001000010100000001101;
	b8 = 0b00000000000000110010100010101001;
	b9 =  0b00000000000000000010100000001101;
	b10 = 0b00000000000000110011000000001101;
	b11 = 0b00000000000000000011100000001010;
	b12 = 0b00000000000100100101000100000100;
	b13 = 0b00000000000000000100100111000101;
	b14 = 0b00000000000000010100101000000101;
	b15 = 0b00000000000011101000001110000100;
	b16 = 0b00000000000000000011101001101001;
	b17 = 0b00000000000000010010100101001101;
	b18 = 0b00000000000000000101001101000000;
	b19 = 0b11111111111111111100100000010110;
	// b20 = 0b00000000000000000000000000010000;

	p = {b0,  b1,  b2,	b3,	 b4,  b5,  b6,	b7,	 b8,  b9,  b10,
		 b11, b12, b13, b14, b15, b16, b17, b18, b19};
	this->d->load(p);
}

Worker::~Worker()
{
	emit finished();
	qDebug() << "Worker destructor called in thread:"
			 << QThread::currentThread();
	delete this->ct;
	delete this->c;
}

void Worker::refreshDram()
{
	qDebug() << "Refreshing Dram";
	emit dram_storage(this->d->view(0, 32));
}

void Worker::refreshCache()
{
	qDebug() << "Refreshing Dram";
	emit cache_storage(this->c->view(0, 7));
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
	emit dram_storage(this->d->view(0, 256));
	emit cache_storage(this->c->view(0, 7));
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
	this->ct->advance(WAIT);
	emit dram_storage(this->d->view(0, 256));
	emit cache_storage(this->c->view(0, 7));
	emit register_storage(this->ct->get_gprs());
	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit if_info(this->if_stage->stage_info());
	emit id_info(this->id_stage->stage_info());
	emit ex_info(this->ex_stage->stage_info());
	emit mm_info(this->mm_stage->stage_info());
	emit wb_info(this->wb_stage->stage_info());
}
