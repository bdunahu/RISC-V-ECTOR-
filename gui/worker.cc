#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent) {}

Worker::~Worker()
{
	emit finished();
	qDebug() << "Worker destructor called in thread:"
			 << QThread::currentThread();
	delete this->ct;
}

void Worker::configure(std::vector<unsigned int> ways, bool is_pipelined)
{
	// this->d = new Dram(DRAM_DELAY);
	// setWays(ways);
	// setSize(size);
	// this->cache_enabled = is_cache_enabled;
	// if (!is_cache_enabled || ways.size() == 0) {
	// 	this->ct = new Controller(wb_stage, this->d, is_pipelined);
	// } else {
	// 	// 0th index cache has largest delay
	// 	for (int i = 0; i < ways.size(); i++) {
	// 		if (i == 0) {
	// 			Cache *cache =
	// 				new Cache(this->d, size[i], ways[i], ways.size());
	// 			this->c.push_back(cache);
	// 		} else {
	// 			Cache *cache = new Cache(
	// 				this->c[i - 1], size[i], ways[i], ways.size() - i);
	// 			this->c.push_back(cache);
	// 		}
	// 	}
	// 	this->ct =
	// 		new Controller(wb_stage, this->c.at(ways.size() - 1), is_pipelined);
	// }
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
