#include "instrDTO.h"
#include "accessor.h"

InstrDTO::InstrDTO()
{
	this->instr_bits = 0;
	this->s1 = 0;
	this->s2 = 0;
	this->s3 = 0;
}

int InstrDTO::get_time_of(Accessor a) { return this->hist[a]; }

signed int InstrDTO::get_instr_bits() { return this->instr_bits; }

signed int InstrDTO::get_s1() { return this->s1; }

signed int InstrDTO::get_s2() { return this->s2; }

signed int InstrDTO::get_s3() { return this->s3; }

Mnemonic InstrDTO::get_mnemonic() { return this->mnemonic; }

void InstrDTO::set_time_of(Accessor a, int i) { this->hist[a] = i; }

void InstrDTO::set_instr_bits(signed int instr) { this->instr_bits = instr; }

void InstrDTO::set_s1(signed int s) { this->s1 = s; }

void InstrDTO::set_s2(signed int s) { this->s2 = s; }

void InstrDTO::set_s3(signed int s) { this->s3 = s; }

void InstrDTO::set_mnemonic(Mnemonic m) { this->mnemonic = m; }
