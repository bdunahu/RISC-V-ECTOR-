#include "dum.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

DUM::DUM(Stage *stage) : Stage(stage) { this->id = IDLE; }

InstrDTO *DUM::advance(Response p)
{
	InstrDTO *r = nullptr;

	if (this->curr_instr && p == WAIT) {
		r = new InstrDTO(*this->curr_instr);
		delete this->curr_instr;
		curr_instr = nullptr;
	}

	return r;
}

void DUM::advance_helper() {}

void DUM::set_curr_instr(InstrDTO *d)
{
	this->curr_instr = d;
}
