#include "instrDTO.h"

InstrDTO::InstrDTO()
{
	this->if_cycle = 0;
	this->instr_bits = 0;
}

int InstrDTO::get_if_cycle() { return this->if_cycle; }

signed int InstrDTO::get_instr_bits() { return this->instr_bits; }

void InstrDTO::set_if_cycle(int cycle) { this->if_cycle = cycle; }

void InstrDTO::set_instr_bits(signed int instr) { this->instr_bits = instr; }
