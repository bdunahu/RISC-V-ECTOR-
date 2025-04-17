#include "instrDTO.h"
#include "accessor.h"

InstrDTO::InstrDTO()
{
	this->instr_bits = 0;
	this->checked_out = -1;
	this->s1 = 0;
	this->s2 = 0;
	this->s3 = 0;
	this->mnemonic = ADD;
	this->type = INV;
	this->pc = 0;
}

signed int InstrDTO::get_instr_bits() { return this->instr_bits; }

signed int InstrDTO::get_checked_out() { return this->checked_out; }

signed int InstrDTO::get_s1() { return this->s1; }

signed int InstrDTO::get_s2() { return this->s2; }

signed int InstrDTO::get_s3() { return this->s3; }

Mnemonic InstrDTO::get_mnemonic() { return this->mnemonic; }

Type InstrDTO::get_type() { return this->type; }

unsigned int InstrDTO::get_pc() { return this->pc; }

void InstrDTO::set_instr_bits(signed int instr) { this->instr_bits = instr; }

void InstrDTO::set_checked_out(signed int checked_out)
{
	this->checked_out = checked_out;
}

void InstrDTO::set_s1(signed int s) { this->s1 = s; }

void InstrDTO::set_s2(signed int s) { this->s2 = s; }

void InstrDTO::set_s3(signed int s) { this->s3 = s; }

void InstrDTO::set_mnemonic(Mnemonic m) { this->mnemonic = m; }

void InstrDTO::set_type(Type t) { this->type = t; }

void InstrDTO::set_pc(unsigned int pc) { this->pc = pc; }
