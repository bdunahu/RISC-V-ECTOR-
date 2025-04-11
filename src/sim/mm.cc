#include "mm.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

MM::MM(Stage *stage) : Stage(stage) { this->id = MEM; }

void
MM::advance_helper()
{
	signed int data;
	unsigned int r;

	switch (this->curr_instr->get_mnemonic()) {
	case LOAD:
		r			 = this->storage->read_word(this->id, this->curr_instr->get_s1(), data);
		this->status = r ? OK : STALLED;
		if (this->status == OK) {
			this->curr_instr->set_s1(data);
		}
		break;

	case STORE:
		// TODO signed issues, we aren't wrapping addresses
		r = this->storage->write_word(
			this->id, this->curr_instr->get_s2(), this->curr_instr->get_s1());
		this->status = r ? OK : STALLED;
		if (this->status != OK) {
			this->status = STALLED;
		}
		break;

	default:
		this->status = OK;
	}
}
