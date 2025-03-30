#include "dum.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"
#include "utils.h"

static Logger *global_log = Logger::getInstance();

DUM::DUM(Stage *stage) : Stage(stage) { this->id = IDLE; }

InstrDTO *DUM::advance(Response p)
{
	InstrDTO *r = curr_instr;

	this->advance_helper();
	if (this->status == OK && p == OK) {
		this->curr_instr->set_time_of(this->id, this->clock_cycle);
		r = new InstrDTO(*this->curr_instr);
		delete curr_instr;
		curr_instr = nullptr;
	}

	return r;
}

void DUM::advance_helper()
{
	if (this->curr_instr)
		global_log->log(
			DEBUG, string_format(
					  "Using bits: %i ", this->curr_instr->get_instr_bits()));
	else
		global_log->log(DEBUG, "curr_instr is null");
}

void DUM::set_curr_instr(InstrDTO *d)
{
	this->curr_instr = d;
	this->status = OK;
}
