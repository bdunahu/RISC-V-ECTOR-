#include "if.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

IF::IF(Stage *stage) : Stage(stage) { this->id = FETCH; }

InstrDTO *IF::advance(Response p)
{
	InstrDTO *r = nullptr;

	this->advance_helper();
	if (this->curr_instr != nullptr && p == OK) {
		// mutual consent
		++this->pc;
		this->curr_instr->set_time_of(this->id, this->clock_cycle);
		r = new InstrDTO(*this->curr_instr);
		delete curr_instr;
		curr_instr = nullptr;
		this->status = STALLED;
	}

	return r;
}

void IF::advance_helper()
{
	Response r;
	signed int bits;

	if (this->curr_instr == nullptr) {
		r = this->storage->read_word(this->id, this->pc, bits);
		if (r == OK) {
			this->status = r;
			this->curr_instr = new InstrDTO();
			this->curr_instr->set_instr_bits(bits);
		}
	}
}
