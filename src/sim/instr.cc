#include "instr.h"
#include <functional>
#include <map>
#include <unordered_map>

// clang-format off
#define INIT_INSTRUCTION(mnemonic, body) \
	{mnemonic, [](signed int &s1, signed int &s2, signed int &s3) { \
		body; \
	}}
// clang-format on

namespace instr
{
// clang-format off
const std::unordered_map<Mnemonic, std::function<void(signed int &s1, signed int &s2, signed int &s3)>>
	// clang-format on
	instr_map = {

		/* R type instructions */
		// TODO these need to be WRAPPED with a function that sets overflow.
		// future note to self, if these are more than 2 lines each, you're
		// doing it wrong
		INIT_INSTRUCTION(ADD, s3 = s1 + s2;),
		INIT_INSTRUCTION(SUB, s3 = s1 - s2;),

		/* I type instructions */

		/* J type instructions */

		/* NOP */
		INIT_INSTRUCTION(NOP, (void)s3; (void)s2; (void)s1;),

};

const std::unordered_map<unsigned int, Mnemonic> mnemonic_map = {
	{0b0000100, ADD},	 {0b0001000, SUB},	 {0b0001100, MUL},
	{0b0010000, QUOT},	 {0b0010100, REM},	 {0b0011000, SFTR},
	{0b0011100, SFTL},	 {0b0100000, AND},	 {0b0100100, OR},
	{0b0101000, NOT},	 {0b0101100, XOR},	 {0b0110000, ADDV},
	{0b0110100, SUBV},	 {0b0111000, MULV},	 {0b0111100, DIVV},
	{0b1000000, CMP},	 {0b1000100, CEV},	 {0b000101, LOAD},
	{0b001001, LOADV},	 {0b0001101, ADDI},	 {0b0010001, SUBI},
	{0b0010101, SFTRI},	 {0b0011101, SFTLI}, {0b0100001, ANDI},
	{0b0100101, ORI},	 {0b0101001, XORI},	 {0b0101101, STORE},
	{0b0110001, STOREV}, {0b0000101, CEV},	 {0b0000101, LOAD},
	{0b0001001, LOADV},	 {0b0001001, LOADV}, {0b0000110, JMP},
	{0b0001010, JRL},	 {0b0001110, JAL},	 {0b0010010, BEQ},
	{0b0010110, BGT},	 {0b0011010, BUF},	 {0b0011110, BOF},
	{0b0100010, PUSH},	 {0b0100110, POP},

};
} // namespace instr
