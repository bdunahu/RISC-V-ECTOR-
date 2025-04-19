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
	unsigned int size_inc;
	Dram *d;
	Storage *s;
	int i;

	if (ways.size() != 0)
		size_inc = MEM_LINE_SPEC / ways.size();
	d = new Dram(DRAM_DELAY);
	s = (Storage *)d;

	this->s.push_front(s);
	d->load(program);

	for (i = ways.size(); i > 0; --i) {
		s = (Storage *)new Cache(
			s, size_inc * (i), ways.at(i - 1), CACHE_DELAY + i);
		this->s.push_front(s);
	}

	this->if_stage = new IF(nullptr);
	this->id_stage = new ID(if_stage);
	this->ex_stage = new EX(id_stage);
	this->mm_stage = new MM(ex_stage);
	this->wb_stage = new WB(mm_stage);
	this->ct =
		new Controller(wb_stage, s, is_pipelined);

	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
}

void Worker::refreshDram()
{
	qDebug() << "Refreshing Dram";
	// emit dram_storage(this->d->view(0, 255));
}

void Worker::refreshCache()
{
	qDebug() << "Refreshing Cache";
	// if(getWays().size() > 0) {
	// 	unsigned int size = this->c.at(getWays().size()-1)->get_size();
	// 	emit cache_storage(this->c.at(getWays().size()-1)->view(0, 1<<size));
	// }
}

void Worker::refreshRegisters()
{
	qDebug() << "Refreshing Registers";
	emit register_storage(this->ct->get_gprs());
}

void Worker::runSteps(int steps)
{
	qDebug() << "Running for " << steps << "steps";
	this->ct->run_for(steps);
	// emit dram_storage(this->d->view(0, 255));
	// emit register_storage(this->ct->get_gprs());
	// emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	// emit if_info(this->if_stage->stage_info());
	// emit id_info(this->id_stage->stage_info());
	// emit ex_info(this->ex_stage->stage_info());
	// emit mm_info(this->mm_stage->stage_info());
	// emit wb_info(this->wb_stage->stage_info());
}
