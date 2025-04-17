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
	emit cache_storage(this->c->view(0, 8));
	emit register_storage(this->ct->get_gprs());

	signed int b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14,
		b15, b16, b17, b18, b19;
	std::vector<signed int> p;

	// I-TYPE              /    /    /   /
	b0 = 0b00000010000000000001000000001101; // ADDI $2 $0 0x200;
	b1 = 0b00000000000000010010100000001101; // ADDI $5 $0 0x1;
	b2 = 0b00000000000000000010100010101101; // STORE $5 0($2);
	// I-TYPE              /    /    /   /
	b3 = 0b00000000000000100010100000001101; // ADDI $5 $0 0x2;
	b4 = 0b00000000000000010010100010101101; // STORE $5 1($2);
	// I-TYPE              /    /    /   /
	b5 = 0b00000000000000110010100000001101; // ADDI $5 $0 0x3;
	b6 = 0b00000000000000100010100010101101; // STORE $5 2($2);
	// I-TYPE              /    /    /   /
	b7 = 0b00000000000001000010100000001101; // ADDI $5 $0 0x4;
	b8 = 0b00000000000000110010100010101101; // STORE $5 3($2);
	// // I-TYPE           /    /    /   /
	b9 = 0b00000000000000000010100000001101; // ADDI $5 $0 0x0;
	// //  I-TYPE           /    /    /   /
	b10 = 0b00000000000000110011000000001101; // ADDI $6 $0 0x3;
	// //  J-TYPE                /    /   /
	b11 = 0b00000000000000000011100000001010; // JRL CHECK
	// //  R-TYPE     /    /    /    /    /
	b12 = 0b00000000000100100101000100000100; // ADD $9 $2 $5;
	// //  I-TYPE           /    /    /   /
	b13 = 0b00000000000000000011101001000101; // LOAD $7 0($9); (RAW HAZARD)!
	//  // I-TYPE           /    /    /   /
	b14 = 0b00000000000000010100001001000101; // LOAD $8 1($9);
	//  // R-TYPE     /    /    /    /    /
	b15 = 0b00000000000011101000001110000100; // ADD $7 $7 $8;
	// I-TYPE               /    /    /   /
	b16 = 0b00000000000000000011101001101101; // STORE $7 0($9);
	b17 = 0b00000000000000010010100101001101; // ADDI $5 $5 0x1;
	//  // R-TYPE     /    /    /    /    /
	b18 = 0b00000000000111100101001101000000; // CMP $6 $5
	//  // J-TYPE                /    /   /
	b19 = 0b11111111111111111100000000010110; // bgt LOOP

	p = {b0,  b1,  b2,	b3,	 b4,  b5,  b6,	b7,	 b8,  b9,
		 b10, b11, b12, b13, b14, b15, b16, b17, b18, b19};
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
	emit cache_storage(this->c->view(0, 8));
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
	emit cache_storage(this->c->view(0, 8));
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
	emit cache_storage(this->c->view(0, 8));
	emit register_storage(this->ct->get_gprs());
	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit if_info(this->if_stage->stage_info());
	emit id_info(this->id_stage->stage_info());
	emit ex_info(this->ex_stage->stage_info());
	emit mm_info(this->mm_stage->stage_info());
	emit wb_info(this->wb_stage->stage_info());
}
