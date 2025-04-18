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
	if (this->curr_instr != nullptr && p == WAIT) {
		// don't increment PC if the PC was just set by wb
		if (this->curr_instr->is_squashed() != 1)
			++this->pc;
		r = new InstrDTO(*this->curr_instr);
		delete curr_instr;
		curr_instr = nullptr;
	}

	return r;
}

std::vector<int> IF::stage_info() {
	std::vector<int> info;
	if(this->curr_instr){
		info.push_back(this->curr_instr->get_pc());
		info.push_back(this->curr_instr->get_instr_bits());
	}
	return info;
}

void IF::advance_helper()
{
	Response r;
	int i;
	signed int bits;

	if (this->curr_instr == nullptr && (this->is_pipelined || this->is_empty)) {
		i = this->storage->read_word(this, this->pc, bits);
		r = i ? OK : STALLED;
		if (r == OK) {
			this->curr_instr = new InstrDTO();
			this->curr_instr->set_instr_bits(bits);
			this->curr_instr->set_pc(this->pc);
			this->is_empty = false;
		}
	}
}
