#include "dum.h"
#include "component.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"
#include "utils.h"

DUM::DUM(Stage *stage) : Stage(stage) { this->id = VOID; }

InstrDTO *
DUM::advance(Response p)
{
	InstrDTO *r = nullptr;

	if (this->curr_instr && p == WAIT) {
		this->curr_instr->set_time_of(this->id, this->clock_cycle);
		r = new InstrDTO(*this->curr_instr);
		delete this->curr_instr;
		curr_instr = nullptr;
	}

	return r;
}

void
DUM::advance_helper()
{
}

void
DUM::set_curr_instr(InstrDTO *d)
{
	this->curr_instr = d;
}
