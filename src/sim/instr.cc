#include "instr.h"
#include <functional>
#include <map>

// clang-format off
#define INIT_INSTRUCTION(type, opcode, body) \
	{type, {{opcode, [](signed int &s1, signed int &s2, signed int &s3) { \
		body; \
	}}}}
// clang-format on

namespace instr
{
// clang-format off
const std::map<int,	std::map<int, std::function<void(signed int &s1, signed int &s2, signed int &s3)>>>
	// clang-format on
	instr_map = {

		/* R type instructions */
		// TODO these need to be WRAPPED with a function that sets overflow.
		// future note to self, if these are more than 2 lines each, you're
		// doing it wrong
		INIT_INSTRUCTION(0b00, 0b00001, s3 = s1 + s2),
		INIT_INSTRUCTION(0b00, 0b00010, s3 = s1 - s2),

		/* I type instructions */

		/* J type instructions */
};
} // namespace instr
