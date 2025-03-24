#include "controller.h"
#include "ex.h"
#include "id.h"
#include "if.h"
#include "mm.h"
#include "response.h"
#include "storage.h"
#include "wb.h"

Controller::Controller(Storage *storage, bool is_pipelined)
	: Stage(nullptr)
{
	this->clock_cycle = 0;
	this->storage = storage;
	this->is_pipelined = is_pipelined;
	this->pc = 0x0;
	this->gprs = {0};

	IF *f = new IF(nullptr);
	ID *d = new ID(f);
	EX *e = new EX(d);
	MM *m = new MM(e);
	WB *w = new WB(m);
	this->next = w;
}

void Controller::run_for(int number)
{
	int i;
	for (i = 0; i < number; ++i) {
		this->advance();
	}
}

int Controller::get_clock_cycle() { return this->clock_cycle; }

std::array<int, GPR_NUM> Controller::get_gprs() { return this->gprs; }

int Controller::get_pc() { return this->pc; }

Response Controller::advance()
{
	this->next->advance();
	++this->clock_cycle;
	return OK;
}
