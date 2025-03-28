#include "instrDTO.h"

InstrDTO::InstrDTO()
{
	this->if_cycle = 0;
	this->id_cycle = 0;
	this->instr_bits = 0;
	this->s1 = 0;
	this->s2 = 0;
	this->s3 = 0;
}

int InstrDTO::get_if_cycle() { return this->if_cycle; }

int InstrDTO::get_id_cycle() { return this->id_cycle; }

signed int InstrDTO::get_instr_bits() { return this->instr_bits; }

signed int InstrDTO::get_s1() { return this->s1; }

signed int InstrDTO::get_s2() { return this->s2; }

signed int InstrDTO::get_s3() { return this->s3; }

void InstrDTO::set_if_cycle(int cycle) { this->if_cycle = cycle; }

void InstrDTO::set_id_cycle(int cycle) { this->id_cycle = cycle; }

void InstrDTO::set_instr_bits(signed int instr) { this->instr_bits = instr; }

void InstrDTO::set_s1(signed int s) { this->s1 = s; }

void InstrDTO::set_s2(signed int s) { this->s2 = s; }

void InstrDTO::set_s3(signed int s) { this->s3 = s; }
