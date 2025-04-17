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

	std::vector<signed int> p;

	p.push_back(0b00000000000000010010100000001101);
	p.push_back(0b00000000000000000000000101100010);
	p.push_back(0b00000000000000010010100101001101);
	p.push_back(0b00000000000000000000000101100010);
	p.push_back(0b00000000000000010010100101001101);
	p.push_back(0b00000000000000000000000101100010);
	p.push_back(0b00000000000000010010100101001101);
	p.push_back(0b00000000000000000000000101100010);
	p.push_back(0b00000000000000000000000101100110);
	p.push_back(0b00000000000000000000000110100110);
	p.push_back(0b00000000000000000000000111100110);
	p.push_back(0b00000000000000000000001000100110);
	p.push_back(0b00000000000000000000000000101110);
	p.push_back(0b00000000000000000000000000101110);
	p.push_back(0b00000000000000000000000000010000);

	// p.push_back(0b00000000000000000010100010001101);
	// p.push_back(0b11111111111101010011000000001101);
	// p.push_back(0b00000000000000000000000110100010);
	// p.push_back(0b00000000000000000011000000001110);
	// p.push_back(0b00000000000000000000000110100110);
	// p.push_back(0b00000000001000100011000000101001);
	// p.push_back(0b00000000000000000000000101100010);
	// p.push_back(0b00000000000000000010100010001101);
	// p.push_back(0b00000000000000010001000010010001);
	// p.push_back(0b11111111111010010011000000001101);
	// p.push_back(0b11111111111111000011000101101001);
	// p.push_back(0b00000000000001000010100111000101);
	// p.push_back(0b11111111111111000010100110000101);
	// p.push_back(0b00000000000011000111001100000100);
	// p.push_back(0b00000000000000000000000110100010);
	// p.push_back(0b00000000000000001010100000001110);
	// p.push_back(0b00000000000000000000000110100110);
	// p.push_back(0b00000000000001000011000101101001);
	// p.push_back(0b00000000000000000001000101001101);
	// p.push_back(0b00000000000000000000000101100110);
	// p.push_back(0b00000000000000000000000000101010);
	// p.push_back(0b00000000000000000000000101100010);
	// p.push_back(0b00000000000000000010100010001101);
	// p.push_back(0b00000000000000010001000010010001);
	// p.push_back(0b00000000010011000011000000001101);
	// p.push_back(0b11111111111111000011000101101001);
	// p.push_back(0b00000000000001000010100111000101);
	// p.push_back(0b11111111111111000010100110000101);
	// p.push_back(0b00000000000011000111001100000100);
	// p.push_back(0b00000000000001000011000101101001);
	// p.push_back(0b00000000000000000001000101001101);
	// p.push_back(0b00000000000000000000000101100110);
	// p.push_back(0b00000000000000000000000000101010);
	// p.push_back(0b00000000000000000000000000010000);
	// p.push_back(0b00000000000000000000000000000000);
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
	emit cache_storage(this->c->view(24, 31));
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
	this->ct->run_for(1);
	emit dram_storage(this->d->view(0, 256));
	emit cache_storage(this->c->view(24, 8));
	emit register_storage(this->ct->get_gprs());
	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit if_info(this->if_stage->stage_info());
	emit id_info(this->id_stage->stage_info());
	emit ex_info(this->ex_stage->stage_info());
	emit mm_info(this->mm_stage->stage_info());
	emit wb_info(this->wb_stage->stage_info());
}
