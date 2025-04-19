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

	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
}

void Worker::runSteps(int steps)
{
	this->ct_mutex.lock();
	qDebug() << "Running for " << steps << "steps";
	this->ct->run_for(steps);
	// TODO move these to separate functions
	emit dram_storage(this->s.back()->view(0, 255));
	if (this->s.size() > 1) {
		emit cache_storage(this->s.at(0)->view(0, 1 << this->size_inc));
	}
	emit register_storage(this->ct->get_gprs());
	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit if_info(this->if_stage->stage_info());
	emit id_info(this->id_stage->stage_info());
	emit ex_info(this->ex_stage->stage_info());
	emit mm_info(this->mm_stage->stage_info());
	emit wb_info(this->wb_stage->stage_info());
	this->ct_mutex.unlock();
}
