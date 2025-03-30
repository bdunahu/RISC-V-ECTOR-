#include "ex.h"
#include "accessor.h"
#include "definitions.h"
#include "instrDTO.h"
#include "response.h"
#include "stage.h"
#include <unordered_map>

// clang-format off
#define INIT_INSTRUCTION(mnemonic, body)					\
	{mnemonic, [this](signed int &s1, signed int &s2) { \
		body; \
	}}
// clang-format on

EX::EX(Stage *stage) : Stage(stage)
{
	this->id = EXEC;
	instr_map = {

		/* R type instructions */
		INIT_INSTRUCTION(
			ADD,
			{
				this->overflow_guard(s1, s2);
				s1 = s1 + s2;
			}),

		INIT_INSTRUCTION(
			SUB,
			{
				this->overflow_guard(s1, -s2);
				s1 = s1 - s2;
			}),

		INIT_INSTRUCTION(MUL, {}),
		INIT_INSTRUCTION(QUOT, {}),
		INIT_INSTRUCTION(REM, {}),
		INIT_INSTRUCTION(SFTR, {}),
		INIT_INSTRUCTION(SFTL, {}),
		INIT_INSTRUCTION(AND, {}),
		INIT_INSTRUCTION(OR, {}),
		INIT_INSTRUCTION(NOT, {}),
		INIT_INSTRUCTION(XOR, {}),
		INIT_INSTRUCTION(ADDV, {}),
		INIT_INSTRUCTION(SUBV, {}),
		INIT_INSTRUCTION(MULV, {}),
		INIT_INSTRUCTION(DIVV, {}),
		INIT_INSTRUCTION(CMP, {}),
		INIT_INSTRUCTION(CEV, {}),

		/* I type instructions */
		INIT_INSTRUCTION(LOAD, {}),
		INIT_INSTRUCTION(LOADV, {}),
		INIT_INSTRUCTION(ADDI, {}),
		INIT_INSTRUCTION(SUBI, {}),
		INIT_INSTRUCTION(SFTRI, {}),
		INIT_INSTRUCTION(SFTL, {}),
		INIT_INSTRUCTION(ANDI, {}),
		INIT_INSTRUCTION(ORI, {}),
		INIT_INSTRUCTION(XORI, {}),
		INIT_INSTRUCTION(STORE, {}),
		INIT_INSTRUCTION(STOREV, {}),

		/* J type instructions */
		INIT_INSTRUCTION(JMP, {}),
		INIT_INSTRUCTION(JRL, {}),
		INIT_INSTRUCTION(JAL, {}),
		INIT_INSTRUCTION(BEQ, {}),
		INIT_INSTRUCTION(BGT, {}),
		INIT_INSTRUCTION(BUF, {}),
		INIT_INSTRUCTION(BOF, {}),
		INIT_INSTRUCTION(PUSH, {}),
		INIT_INSTRUCTION(POP, {}),

		/* NOP */
		INIT_INSTRUCTION(NOP, (void)s2; (void)s1;),
	};
}

InstrDTO *EX::advance(Response p) { return nullptr; }

void EX::overflow_guard(signed int a, signed int b)
{
	if (a >= 0 && b >= 0 && (a > MAX_INT - b)) {
		this->set_condition(OF, true);
		this->set_condition(UF, false);
	} else if (a < 0 && b < 0 && (a < (-MAX_INT) - 1 - b)) {
		this->set_condition(OF, false);
		this->set_condition(UF, true);
	} else {
		this->set_condition(OF, false);
		this->set_condition(UF, false);
	}
}
