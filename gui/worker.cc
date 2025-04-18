#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent) {}

void Worker::configure(std::vector<int> ways, std::vector<int> size, bool is_pipelined, bool is_cache_enabled) {
	this->d = new Dram(ways.size()+10);
	setWays(ways);
	setSize(size);
	qDebug() << "is cache enabled:" << is_cache_enabled;
	qDebug() << "is pipelined:" << is_pipelined;
	this->cache_enabled = is_cache_enabled;
	if (!is_cache_enabled || ways.size() == 0) {
		this->ct = new Controller(wb_stage, this->d, is_pipelined);
	} else {
		// 0th index cache has largest delay
		for(int i=0;i<ways.size();i++) {
			if(i==0){
				Cache* cache = new Cache(this->d, size[i], ways[i], ways.size());
				this->c.push_back(cache);
			} else {
				Cache* cache = new Cache(this->c[i-1], size[i], ways[i], ways.size()-i);
				this->c.push_back(cache);
			}
		}
		this->ct = new Controller(wb_stage, this->c.at(ways.size()-1), is_pipelined);
	}
	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
}

void Worker::setWays(std::vector<int> ways) {
	this->cache_ways = ways;
}

void Worker::setSize(std::vector<int> size) {
	this->cache_size = size;
}

std::vector<int> Worker::getWays() {
	return this->cache_ways;
}

std::vector<int> Worker::getSize() {
	return this->cache_size;
}

void Worker::doWork()
{
	qDebug() << "Initializing...";

	this->if_stage = new IF(nullptr);
	this->id_stage = new ID(if_stage);
	this->ex_stage = new EX(id_stage);
	this->mm_stage = new MM(ex_stage);
	this->wb_stage = new WB(mm_stage);
}

Worker::~Worker()
{
	emit finished();
	qDebug() << "Worker destructor called in thread:"
			 << QThread::currentThread();
	delete this->ct;
	for(Cache *cache: this->c) {
		delete cache;
	}
}

void Worker::loadProgram(std::vector<signed int> p) {
	this->d->load(p);
}

void Worker::refreshDram()
{
	qDebug() << "Refreshing Dram";
	emit dram_storage(this->d->view(0, 255));
}

void Worker::refreshCache()
{
	qDebug() << "Refreshing Cache";
	if(getWays().size() > 0) {
		unsigned int size = this->c.at(getWays().size()-1)->get_size();
		emit cache_storage(this->c.at(getWays().size()-1)->view(0, 1<<size));
	}
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
	emit dram_storage(this->d->view(0, 255));
	if(this->cache_enabled && getWays().size() > 0) {
		unsigned int size = this->c.at(getWays().size()-1)->get_size();
		emit cache_storage(this->c.at(getWays().size()-1)->view(0, 1<<size));
	}
	emit register_storage(this->ct->get_gprs());
	emit clock_cycles(this->ct->get_clock_cycle(), this->ct->get_pc());
	emit if_info(this->if_stage->stage_info());
	emit id_info(this->id_stage->stage_info());
	emit ex_info(this->ex_stage->stage_info());
	emit mm_info(this->mm_stage->stage_info());
	emit wb_info(this->wb_stage->stage_info());
}
