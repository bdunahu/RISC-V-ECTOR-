#include "wb.h"
#include "accessor.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"
#include <array>
#include <algorithm>

WB::WB(Stage *stage) : Stage(stage) { this->id = WRITE; }

void WB::advance_helper()
{
	if (this->curr_instr->get_mnemonic() != NOP &&
		this->curr_instr->get_type() != INV) {
		if (this->curr_instr->get_checked_out() > 0)
			this->write_handler();
		else if (this->should_jump())
			this->jump_handler();
	}
	this->status = OK;
}

void WB::write_handler()
{
	signed int reg;

	if (this->checked_out.size() < 1)
		throw std::runtime_error("instruction tried to pop a register out of "
								 "an empty queue during writeback.");

	this->checked_out.pop_front();
	reg = this->curr_instr->get_checked_out();
	this->store_register(reg, this->curr_instr->get_s1());
}

void WB::jump_handler()
{
	if (this->curr_instr->get_s1() > 0) {
		if (this->curr_instr->get_mnemonic() == JAL)
			this->gprs[1] = this->pc + 1;
		this->pc = this->curr_instr->get_s1();
		this->checked_out = {};
		this->next->squash();
	}
}

bool WB::should_jump()
{
	Type t;

	t = this->curr_instr->get_type();
	return t == J;
}
