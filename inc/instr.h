#ifndef INSTR_H
#define INSTR_H
#include <functional>
#include <iostream>
#include <unordered_map>

enum Mnemonic {
	ADD,
	SUB,
	MUL,
	QUOT,
	REM,
	SFTR,
	SFTL,
	AND,
	OR,
	NOT,
	XOR,
	ADDV,
	SUBV,
	MULV,
	DIVV,
	CMP,
	CEV,
	LOAD,
	LOADV,
	ADDI,
	SUBI,
	SFTRI,
	SFTLI,
	ANDI,
	ORI,
	XORI,
	STORE,
	STOREV,
	JMP,
	JRL,
	JAL,
	BEQ,
	BGT,
	BUF,
	BOF,
	PUSH,
	POP,
	NOP,
};

enum Type {
	R,
	I,
	J,
	INV
};

namespace instr
{
// clang-format off
	extern const std::unordered_map<unsigned int, Mnemonic> mnemonic_map;
// clang-format on
} // namespace instr

#endif /* INSTR_H_INCLUDED */
