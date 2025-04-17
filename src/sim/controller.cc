#include "controller.h"
#include "ex.h"
#include "response.h"
#include "storage.h"

Controller::Controller(Stage *stage, Storage *storage, bool is_pipelined)
	: Stage(stage)
{
	this->clock_cycle = 1;
	this->storage = storage;
	this->is_pipelined = is_pipelined;
	this->is_empty = true;
	this->pc = 0x0;
	this->checked_out = {};
	this->gprs = {0};
	this->gprs.at(2) = MEM_WORDS; // set the stack pointer
	// grant side-door access
	this->id = SIDE;
}

void Controller::run_for(int number)
{
	int i;
	for (i = 0; i < number; ++i) {
		try {
			this->advance(WAIT);
		} catch (HaltException &e) {
			break;
		}
	}
}

int Controller::get_clock_cycle() { return this->clock_cycle; }

std::array<int, GPR_NUM> Controller::get_gprs() { return this->gprs; }

int Controller::get_pc() { return this->pc; }

InstrDTO *Controller::advance(Response p)
{
	InstrDTO *r;
	r = this->next->advance(p);
	++this->clock_cycle;

	if (r)
		this->is_empty = true;

	return r;
}

void Controller::advance_helper()
{
	// TODO: check halt condition and call UI to refresh
}
