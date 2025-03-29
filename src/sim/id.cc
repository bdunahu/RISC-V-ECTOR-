#include "id.h"
#include "accessor.h"
#include "instr.h"
#include "instrDTO.h"
#include "logger.h"
#include "response.h"
#include "stage.h"

ID::ID(Stage *stage) : Stage(stage) { this->id = DCDE; }

Response ID::advance(InstrDTO &next_instr, Response p)
{
	Response r;
	r = OK;
	// signed int s1, s2, s3;
	// Mnemonic m;

	// s1 = next_instr.get_instr_bits();

	// get_instr_fields(s1, s2, s3, m);
	return r;
}

// TODO this function is ugly
void ID::get_instr_fields(
	signed int &s1, signed int &s2, signed int &s3, Mnemonic &m)
{
	unsigned int type, s0b, s1b, s2b;
	this->split_instr(s1, type, m);

	// define the parsing bounds
	s0b = REG_SIZE;
	switch (type) {
	case 0b00:
		// R-TYPE
		s1b = s0b + REG_SIZE;
		s2b = s1b + REG_SIZE;
		break;
	case 0b01:
		// I-TYPE
		s1b = s0b + REG_SIZE;
		s2b = WORD_SPEC;
		break;
	case 0b10:
		// J-TYPE
		s1b = WORD_SPEC;
	}

	if (type != 0b10)
		s3 = GET_MID_BITS(s1, s1b, s2b);

	s2 = GET_MID_BITS(s1, s0b, s1b);
	s1 = GET_LS_BITS(s1, s0b);
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
