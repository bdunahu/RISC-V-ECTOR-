#include "mm.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

MM::MM(Stage *stage) : Stage(stage) { this->id = MEM; }

void MM::advance_helper()
{
	signed int data;
	int i;

	switch (this->curr_instr->get_mnemonic()) {
	case LOAD:
		i = this->storage->read_word(this, this->curr_instr->get_s1(), data);
		this->status = i ? OK : STALLED;
		if (this->status == OK) {
			this->curr_instr->set_s1(data);
		} else
			this->status = STALLED;
		break;

	case STORE:
		// TODO signed issues, we aren't wrapping addresses
		i = this->storage->write_word(
			this, this->curr_instr->get_s2(), this->curr_instr->get_s1());
		this->status = i ? OK : STALLED;
		if (this->status != OK) {
			this->status = STALLED;
		}
		break;

	default:
		this->status = OK;
	}
}
