#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent) {}

void Worker::doWork() {
    qDebug() << "Initializing...";
    this->d = new Dram(0);
	this->c = new Cache(this->d, 0);
    this->if_stage = new IF(nullptr);
	this->id_stage = new ID(if_stage);
	this->ex_stage = new EX(id_stage);
    this->mm_stage = new MM(ex_stage);
    this->wb_stage = new WB(mm_stage);
    this->ct = new Controller(wb_stage, this->c, true);
    
    emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
    emit dram_storage(this->d->view(0,32));
    emit cache_storage(this->c->view(0,8));
    emit register_storage(this->ct->get_gprs());
    
    signed int w;
    w = 0x11223344;
    this->d->write_word(MEM, w, 0x0);
    this->c->write_word(MEM, w, 0x0);
    this->ct->set_gprs(0, w);
}

Worker::~Worker() {
    emit finished();
    qDebug() << "Worker destructor called in thread:" << QThread::currentThread();
    delete this->ct;
	delete this->c;
}


void Worker::refreshDram() {
    qDebug() << "Refreshing Dram";
    emit dram_storage(this->d->view(0,32));
}

void Worker::refreshCache() {
    qDebug() << "Refreshing Dram";
    emit cache_storage(this->c->view(0,8));
}

void Worker::refreshRegisters() {
    qDebug() << "Refreshing Registers";
    emit register_storage(this->ct->get_gprs());
}

void Worker::runSteps(int steps) {
    qDebug() << "Running for steps: " << steps;
    this->ct->run_for(steps);
    emit dram_storage(this->d->view(0,32));
    emit cache_storage(this->c->view(0,8));
    emit register_storage(this->ct->get_gprs());
    emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
}

void Worker::runStep() {
    qDebug() << "Running for 1 step " ;
    this->ct->advance(OK);
    emit dram_storage(this->d->view(0,32));
    emit cache_storage(this->c->view(0,8));
    emit register_storage(this->ct->get_gprs());
    emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
}