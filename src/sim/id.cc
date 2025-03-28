#include "id.h"
#include "accessor.h"
#include "instrDTO.h"
#include "logger.h"
#include "response.h"
#include "stage.h"
#include "utils.h"

static Logger *global_log = Logger::getInstance();

ID::ID(Stage *stage) : Stage(stage) { this->id = DCDE; }

Response ID::advance(InstrDTO &i) { Response r; }

void ID::get_instr_fields(
	signed int &s1,
	signed int &s2,
	signed int &s3,
	unsigned int &type,
	unsigned int &opcode)
{
	// unsigned int &opcode;
	int opcode_bits;

	type = GET_LS_BITS(s1, TYPE_SIZE);
	opcode_bits = (type == 0b0) ? R_OPCODE_SIZE : OPCODE_SIZE;

	switch (type) {
	case 0:
		// R-TYPE
		opcode += GET_MID_BITS(s1, TYPE_SIZE, TYPE_SIZE + opcode_bits);
		s3 = GET_MID_BITS(
			s1, TYPE_SIZE + opcode_bits + (REG_SIZE * 2),
			TYPE_SIZE + opcode_bits + (REG_SIZE * 3));
		s2 = GET_MID_BITS(
			s1, TYPE_SIZE + opcode_bits + REG_SIZE,
			TYPE_SIZE + opcode_bits + (REG_SIZE * 2));
		s1 = GET_MID_BITS(
			s1, TYPE_SIZE + opcode_bits,
			TYPE_SIZE + opcode_bits + REG_SIZE);
		break;
	case 1:
		// I-TYPE
		opcode = GET_MID_BITS(s1, TYPE_SIZE, TYPE_SIZE + opcode_bits);
		s3 = GET_MID_BITS(
			s1, TYPE_SIZE + opcode_bits + (REG_SIZE * 2), WORD_SPEC);
		s2 = GET_MID_BITS(
			s1, TYPE_SIZE + opcode_bits + REG_SIZE,
			TYPE_SIZE + opcode_bits + (REG_SIZE * 2));
		s1 = GET_MID_BITS(
			s1, TYPE_SIZE + opcode_bits, TYPE_SIZE + opcode_bits + REG_SIZE);
		break;
	case 2:
		// J-TYPE
		opcode = GET_MID_BITS(s1, TYPE_SIZE, TYPE_SIZE + opcode_bits);
		s2 = GET_MID_BITS(s1, TYPE_SIZE + OPCODE_SIZE + REG_SIZE, WORD_SPEC);
		s1 = GET_MID_BITS(
			s1, TYPE_SIZE + OPCODE_SIZE, TYPE_SIZE + OPCODE_SIZE + REG_SIZE);
		break;
	default:
		global_log->log(
			DEBUG,
			string_format("%s returning invalid type: %d", __FUNCTION__, type));
	}
}
