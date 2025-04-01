#include "wb.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"

WB::WB(Stage *stage) : Stage(stage) { this->id = WRITE; }

void WB::advance_helper() {
    if(this -> curr_instr) {
        if(this->curr_instr->get_type() == R || this->curr_instr->get_type() == I){ 
            if(this->checked_out.size() > 0) {
                signed int reg = this->checked_out.front();
                this->checked_out.pop_front();
                if(reg >= GPR_NUM){
                    // TODO: handle vector instructions
                } else {
                    if(this->curr_instr->get_mnemonic() != STORE && this->curr_instr->get_mnemonic() != STOREV){
                        this->gprs[reg] = this->curr_instr->get_s1();
                    }
                }
            }
        } else if (this->curr_instr->get_type() == J) {
            // TODO:handle push pop
            // branch taken
            if(this->pc != this->curr_instr->get_s1()) {
                if(this->curr_instr->get_mnemonic() == JAL){
                    // set link register to next instruction
                    this->gprs[1] = this->pc + 1;
                }
                this->pc = this->curr_instr->get_s1();  
                //clear pending registers and squash pipeline
                this->checked_out = {};
                this->next->squash();
            }
        }
    }
    this->status = OK;
}
