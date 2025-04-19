#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include "controller.h"
#include "dram.h"
#include "cache.h"
#include "id.h"
#include "if.h"
#include "ex.h"
#include "mm.h"
#include "wb.h"

class Worker : public QObject {
    Q_OBJECT

private:
    std::vector<Storage*> s;
    std::vector<Stage*> p;
    Controller *ct;

public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

public slots:
    void refreshDram();
    void refreshCache();
    void refreshRegisters();
    void runSteps(int steps);
    void configure(std::vector<unsigned int> ways, bool is_pipelined);

signals:
    void clock_cycles(int value, int pc);
    void dram_storage(const std::vector<std::array<signed int, LINE_SIZE>> data);
    void cache_storage(const std::vector<std::array<signed int, LINE_SIZE>> data);
    void register_storage(const std::array<int, GPR_NUM> data);
    void if_info(const std::vector<int> info);
    void id_info(const std::vector<int> info);
    void ex_info(const std::vector<int> info);
    void mm_info(const std::vector<int> info);
    void wb_info(const std::vector<int> info);
    void finished();
};

#endif // WORKER_H
