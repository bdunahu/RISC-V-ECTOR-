#include "mm.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

MM::MM(Stage *stage) : Stage(stage) { this->id = MEM; } 

void MM::advance_helper() {
	Response r;
	signed int data;
	if(this->curr_instr){
		if (this->curr_instr->get_mnemonic() == LOAD) {
			r = this->storage->read_word(this->id, this->curr_instr->get_s1(), data);
			if(r == OK){
				this->status = OK;
				this->curr_instr->set_s2(data);
			}
		} else if (this->curr_instr->get_mnemonic() == STORE) {
			r = this->storage->write_word(this->id, this->curr_instr->get_s2(), this->curr_instr->get_s1());
			if(r == OK){
				this->status = OK;
			}
		} else {
			// Mem has no work so just forward the instruction to WB
			this->status = OK;
		}
	} 
}
