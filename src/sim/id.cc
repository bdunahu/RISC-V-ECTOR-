#include "id.h"
#include "accessor.h"
#include "instr.h"
#include "instrDTO.h"
#include "logger.h"
#include "response.h"
#include "stage.h"
#include <algorithm>

ID::ID(Stage *stage) : Stage(stage) { this->id = DCDE; }

Response ID::advance(InstrDTO &next_instr, Response p)
{
	Response r;
	signed int s1, s2, s3;
	Mnemonic m;

	s1 = next_instr.get_instr_bits();

	get_instr_fields(s1, s2, s3, m);
	return r;
}

void ID::decode_R_type(signed int &s1, signed int &s2, signed int &s3)
{
	unsigned int s0b, s1b, s2b;
	Response r1, r2;

	s0b = REG_SIZE;
	s1b = s0b + REG_SIZE;
	s2b = s1b + REG_SIZE;
	s3 = GET_MID_BITS(s1, s1b, s2b);
	s2 = GET_MID_BITS(s1, s0b, s1b);
	s1 = GET_LS_BITS(s1, s0b);

	r1 = this->read_guard(s1);
	r2 = this->read_guard(s2);
	this->write_guard(s3);

	this->status = std::max(r1, r2);
}
void ID::decode_I_type(signed int &s1, signed int &s2, signed int &s3)
{
	unsigned int s0b, s1b, s2b;

	s0b = REG_SIZE;
	s1b = s0b + REG_SIZE;
	s2b = WORD_SPEC;
	s3 = GET_MID_BITS(s1, s1b, s2b);
	s2 = GET_MID_BITS(s1, s0b, s1b);
	s1 = GET_LS_BITS(s1, s0b);

	this->status = this->read_guard(s1);
	this->write_guard(s2);
}

void ID::decode_J_type(signed int &s1, signed int &s2)
{
	unsigned int s0b, s1b;

	s0b = REG_SIZE;
	s1b = WORD_SPEC;
	s2 = GET_MID_BITS(s1, s0b, s1b);
	s1 = GET_LS_BITS(s1, s0b);

	this->status = this->read_guard(*&s1);
}

// TODO this function is ugly
void ID::get_instr_fields(
	signed int &s1, signed int &s2, signed int &s3, Mnemonic &m)
{
	unsigned int type;
	this->split_instr(s1, type, m);

	switch (type) {
	case 0b00:
		this->decode_R_type(s1, s2, s3);
		break;
	case 0b01:
		this->decode_I_type(s1, s2, s3);
		break;
	case 0b10:
		this->decode_J_type(s1, s2);
		break;
	}
}

void ID::split_instr(signed int &raw, unsigned int &type, Mnemonic &m)
{
	unsigned int opcode, opcode_size;

	type = GET_LS_BITS(raw, TYPE_SIZE);
	opcode_size = (type == 0b0) ? R_OPCODE_SIZE : OPCODE_SIZE;
	opcode = GET_MID_BITS(raw, TYPE_SIZE, TYPE_SIZE + opcode_size);
	try {
		m = instr::mnemonic_map.at((opcode << TYPE_SIZE) + type);
	} catch (std::out_of_range const &) {
		m = NOP;
	}

	raw = (unsigned int)raw >> (TYPE_SIZE + opcode_size);
}

Response ID::read_guard(signed int &v)
{
	Response r;
	if (this->is_checked_out(v))
		r = BLOCKED;
	else {
		r = OK;
		v = this->dereference_register(v);
	}
	return r;
}

void ID::write_guard(signed int &v)
{
	this->checked_out.push_back(v);
	v = this->dereference_register(v);
}
