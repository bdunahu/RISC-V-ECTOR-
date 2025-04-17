#include "id.h"
#include "accessor.h"
#include "instr.h"
#include "instrDTO.h"
#include "logger.h"
#include "response.h"
#include "stage.h"

ID::ID(Stage *stage) : Stage(stage) { this->id = DCDE; }

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
		r = STALLED;
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

void ID::advance_helper()
{
	signed int s1, s2, s3;
	Mnemonic m;
	Type t;

	if (curr_instr->get_mnemonic() == NOP)
		this->status = OK;
	else {
		s1 = curr_instr->get_instr_bits();
		get_instr_fields(s1, s2, s3, m, t);
		if (this->status == OK) {
			curr_instr->set_s1(s1);
			curr_instr->set_s2(s2);
			curr_instr->set_s3(s3);
			curr_instr->set_mnemonic(m);
			curr_instr->set_type(t);
		}
	}
}

void ID::get_instr_fields(
	signed int &s1, signed int &s2, signed int &s3, Mnemonic &m, Type &t)
{
	unsigned int type;
	this->split_instr(s1, type, m);

	switch (type) {
	case 0b00:
		t = R;
		this->decode_R_type(s1, s2, s3);
		break;
	case 0b01:
		t = I;
		this->decode_I_type(s1, s2, s3, m);
		break;
	case 0b10:
		t = J;
		this->decode_J_type(s1, s2, s3);
		break;
	case 0b11:
		t = INV;
		this->status = OK;
	}
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
	this->status = (r1 == OK && r2 == OK) ? OK : STALLED;

	if (this->status == OK)
		this->write_guard(s3);
}

void ID::decode_I_type(
	signed int &s1, signed int &s2, signed int &s3, Mnemonic &m)
{
	unsigned int s0b, s1b, s2b;
	Response r1;

	s0b = REG_SIZE;
	s1b = s0b + REG_SIZE;
	s2b = WORD_SPEC - LINE_SPEC - OPCODE_SIZE;
	s3 = GET_BITS_SIGN_EXTEND(s1, s1b, s2b);
	s2 = GET_MID_BITS(s1, s0b, s1b);
	s1 = GET_LS_BITS(s1, s0b);

	r1 = this->read_guard(s1);
	if (m != STORE && m != STOREV) {
		this->status = r1;
		if (r1 == OK)
			this->write_guard(s2);
	} else
		this->status = (this->read_guard(s2) == OK && r1 == OK) ? OK : STALLED;
}

void ID::decode_J_type(signed int &s1, signed int &s2, signed int &s3)
{
	unsigned int s0b, s1b;

	s0b = REG_SIZE;
	s1b = WORD_SPEC - LINE_SPEC - OPCODE_SIZE;
	s3 = 0;
	s2 = GET_BITS_SIGN_EXTEND(s1, s0b, s1b);
	s1 = GET_LS_BITS(s1, s0b);

	std::cout << "s1: " << s1 << ", s2: " << s2 << ", s3: " << s3 << std::endl;
	this->status = this->read_guard(*&s1);
}

std::vector<int> ID::stage_info() {
	std::vector<int> info;
	if(this->curr_instr){
		info.push_back(this->curr_instr->get_pc());
		info.push_back(this->curr_instr->get_instr_bits());
	}
	return info;
}
