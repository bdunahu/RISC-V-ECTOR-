#include "mm.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

MM::MM(Stage *stage) : Stage(stage) { this->id = MEM; }

void MM::advance_helper()
{
	signed int data;

	std::cout << "mem" << this->curr_instr->get_s2() << std::endl;
	switch (this->curr_instr->get_mnemonic()) {
	case LOAD:
		this->status = this->storage->read_word(
			this->id, this->curr_instr->get_s1(), data);
		if (this->status == OK) {
			this->curr_instr->set_s2(data);
		} else
			this->status = STALLED;
		break;

	case STORE:
		// TODO signed issues, we aren't wrapping addresses
		this->status = this->storage->write_word(
			this->id, this->curr_instr->get_s2(), this->curr_instr->get_s1());
		if (this->status != OK) {
			this->status = STALLED;
		}
		break;

	default:
		this->status = OK;
	}
}
