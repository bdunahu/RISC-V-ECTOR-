// Simulator for the RISC-V[ECTOR] mini-ISA
// Copyright (C) 2025 Siddarth Suresh
// Copyright (C) 2025 bdunahu

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
    Cache *c;
    Dram *d;
    Controller *ct;
    ID *id_stage;
    IF *if_stage;
    EX *ex_stage;
    MM *mm_stage;
    WB *wb_stage;

public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();

public slots:
    void doWork();
    void refreshDram();
    void loadProgram(std::vector<signed int> p);
    void refreshCache();
    void refreshRegisters();
    void runSteps(int steps);
    void runStep();

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
