#ifndef WORKER_H
#define WORKER_H

#include "cache.h"
#include "controller.h"
#include "dram.h"
#include "ex.h"
#include "id.h"
#include "if.h"
#include "mm.h"
#include "wb.h"
#include <QDebug>
#include <QObject>
#include <QThread>
#include <deque>

class Worker : public QObject
{
	Q_OBJECT

  private:
	/**
	 * The storage objects, stored smallest to largest.
	 */
	std::deque<Storage *> s;
	IF *if_stage;
	ID *id_stage;
	EX *ex_stage;
	MM *mm_stage;
	WB *wb_stage;
	Controller *ct;
	/**
	 * The size each progressive cache level increases by.
	 */
	unsigned int size_inc;

  public:
	explicit Worker(QObject *parent = nullptr);
	~Worker();

  public slots:
	void refreshDram();
	void refreshCache();
	void refreshRegisters();
	void runSteps(int steps);
	void configure(
		std::vector<unsigned int> ways,
		std::vector<signed int> program,
		bool is_pipelined);

  signals:
	void clock_cycles(int value, int pc);
	void
	dram_storage(const std::vector<std::array<signed int, LINE_SIZE>> data);
	void
	cache_storage(const std::vector<std::array<signed int, LINE_SIZE>> data);
	void register_storage(const std::array<int, GPR_NUM> data);
	void if_info(const std::vector<int> info);
	void id_info(const std::vector<int> info);
	void ex_info(const std::vector<int> info);
	void mm_info(const std::vector<int> info);
	void wb_info(const std::vector<int> info);
	void finished();
};

#endif // WORKER_H
